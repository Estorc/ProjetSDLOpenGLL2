#include "../../raptiquax.h"
#include "socket.h"
#include <errno.h>

#define MIN(x,y) x < y ? x : y

#pragma region LOW LEVEL

int initiate_socket() {
    #ifdef _WIN32
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            PRINT_ERROR("Failed to initiate Winsock\n");
            return 1;
        }
    #endif
    return 0;
}

void close_socket(int sock) {
    shutdown(sock, 2);
    CLOSESOCKET(sock);
    #ifdef _WIN32
        WSACleanup();
    #endif
}

struct socket_request_listener create_socket_request_listener(int sock) {
    struct socket_request_listener listener;
    FD_ZERO(&listener.readfds);
    FD_SET(sock, &listener.readfds);

    listener.timeout.tv_sec = 0;
    listener.timeout.tv_usec = 0;
    return listener;
}


int socket_request_listen(struct socket_request_listener * listener, int server_sock, int timeout) {
    fd_set tmpfds = listener->readfds;
    listener->timeout.tv_usec = timeout;
    int ready = select(server_sock + 1, &tmpfds, NULL, NULL, &listener->timeout);

    if (ready == -1) {
        perror("select");
        return 0;
    }

    if (FD_ISSET(server_sock, &tmpfds)) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);

        if (client_sock == -1) {
            perror("accept");
            return 0;
        }

        return client_sock;
    }
    return 0;
}


int socket_request_receive(struct socket_request_listener * listener, int client_sock, char * buffer, int size, int timeout, int flags) {
    fd_set tmpfds = listener->readfds;
    listener->timeout.tv_usec = timeout;
    int ready = select(client_sock + 1, &tmpfds, NULL, NULL, &listener->timeout);

    if (ready == -1) {
        perror("select");
        return -1;
    }

    if (FD_ISSET(client_sock, &tmpfds)) {
        memset(buffer, 0, size);
        int lg = recv(client_sock, buffer, size, flags);
        return lg;
    }
    return -1;
}

#pragma region HIGH LEVEL

struct peer *create_peer(int socket) {
    struct peer *peer = malloc(sizeof(struct peer));
    peer->socket = socket;
    peer->listener = create_socket_request_listener(socket);
    peer->incoming_buffer = NULL;
    peer->ping = 0;
    peer->handlers = NULL;
    peer->handler_count = 0;
    return peer;
}

void free_peer(struct peer *peer) {
    free(peer->incoming_buffer);
    for (int i = 0; i < peer->handler_count; i++) {
        free(peer->handlers[i]->msg);
        free(peer->handlers[i]);
    }
    free(peer->handlers);
    free(peer);
}

int receive_message(void *p, char **buffer, int size, int timeout, int flags) {

    struct peer *peer = (struct peer *)p;

    free(*buffer);
    *buffer = malloc(sizeof(char) * (size + 1));
    int bytes_received = socket_request_receive(&peer->listener, peer->socket, *buffer, size, timeout, flags);
    if (bytes_received != -1) {
        if (peer->incoming_buffer && *peer->incoming_buffer) {
            bytes_received += strlen(peer->incoming_buffer);
            char *final_buffer = malloc(sizeof(char) * (bytes_received + 1));  
            strcpy(final_buffer, peer->incoming_buffer);
            strncat(final_buffer, *buffer, 512);
            free(*buffer);
            *buffer = final_buffer;
            free(peer->incoming_buffer);
            peer->incoming_buffer = NULL;
        }
        char * last_msg = strrchr(*buffer, MSG_TERMINATOR);
        if (!last_msg) {
            free(peer->incoming_buffer);
            peer->incoming_buffer = strdup(*buffer);
            (*buffer)[0] = 0;
        } else if (last_msg[1] != 0) {
            free(peer->incoming_buffer);
            peer->incoming_buffer = strdup(last_msg + 1);
            last_msg[1] = 0;
        }
    }

    return bytes_received;
}


int read_messages(int bytes_received, char *msg_buffer, void * p) {
    struct peer *peer = (struct peer *)p;
    char *context;
    char *msg = strtok_r(msg_buffer, "|", &context);

    for (; msg; msg = strtok_r(NULL, "|", &context)) {
        for (int i = 0; i < peer->handler_count; i++) {
            if (peer->handlers[i]->msg == NULL || strncmp(peer->handlers[i]->msg, msg, strlen(peer->handlers[i]->msg)) == 0) {
                if (peer->handlers[i]->callback(strlen(msg), msg, p, peer->handlers[i]->data)) {
                    return 1;
                }
                if (peer->handlers[i]->lifespan > 0) {
                    peer->handlers[i]->lifespan--;
                    if (peer->handlers[i]->lifespan == 0) {
                        remove_message_handler_by_index(peer, i);
                    }
                }
            }
        }
    }

    return 0;
}

int send_message(int socket, const char *message, int flags) {
    size_t message_length = strlen(message);
    size_t bytes_sent = 0;
    while (bytes_sent < message_length) {
        ssize_t result = send(socket, message + bytes_sent, MIN(512, message_length), flags);
        if (result == -1) {
            switch (errno) {
                case EPIPE:
                case ECONNRESET:
                    PRINT_ERROR("Connection lost, closing socket\n");
                    break;
                case 0:
                    break;
                default:
                    perror("send failed");
                    PRINT_ERROR("Failed to send message\n");
            }
            return -1;
        }
        bytes_sent += result;
    }
    send(socket, "|", 1, flags);
    return 0; // Success
}

void remove_message_handler_by_index(void *p, int i) {
    struct peer *peer = (struct peer *)p;

    free(peer->handlers[i]->msg);
    free(peer->handlers[i]);
    for (int j = i; j < peer->handler_count - 1; j++) {
        peer->handlers[j] = peer->handlers[j + 1];
    }
    peer->handler_count--;
}

void remove_message_handling(void *p, char *msg) {
    struct peer *peer = (struct peer *)p;
    for (int i = 0; i < peer->handler_count; i++) {
        if (peer->handlers[i]->msg == NULL || strcmp(peer->handlers[i]->msg, msg) == 0) {
            remove_message_handler_by_index(peer, i);
        }
    }
}


void add_message_handler(void *p, char *msg, int (*callback)(int, char *, void *, void **), int lifespan, int argc, void **data) {
    struct peer *peer = (struct peer *)p;
    struct message_handler *handler = malloc(sizeof(struct message_handler));
    if (msg == NULL) {
        handler->msg = NULL;
    } else {
        handler->msg = strdup(msg);
    }
    handler->callback = callback;
    handler->lifespan = lifespan;
    if (argc > 0) {
        handler->data = malloc(sizeof(void *) * argc);
        for (int i = 0; i < argc; i++) {
            handler->data[i] = data[i];
        }
    } else {
        handler->data = NULL;
    }

    peer->handlers = realloc(peer->handlers, sizeof(struct message_handler *) * (peer->handler_count + 1));
    peer->handlers[peer->handler_count] = handler;
    peer->handler_count++;
}