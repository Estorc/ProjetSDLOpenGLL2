#include "../../raptiquax.h"
#include "socket.h"

#pragma region LOW LEVEL

int initiate_socket() {
    #ifdef _WIN32
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            PRINT_ERROR("Failed to initiate Winsock\n");
            exit(0);
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
        exit(0);
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
        exit(0);
    }

    if (FD_ISSET(client_sock, &tmpfds)) {
        memset(buffer, 0, size);
        int lg = recv(client_sock, buffer, size, flags);
        return lg;
    }
    return -1;
}

#pragma region HIGH LEVEL

int receive_message(void *p, char **buffer, int size, int timeout, int flags) {

    struct peer *peer = (struct peer *)p;

    free(*buffer);
    *buffer = malloc(sizeof(char) * size + 1);
    int bytes_received = socket_request_receive(&peer->listener, peer->socket, *buffer, size, timeout, flags);
    if (bytes_received != -1) {
        if (peer->incoming_buffer && *peer->incoming_buffer) {
            bytes_received += strlen(peer->incoming_buffer);
            char *final_buffer = malloc(sizeof(char) * bytes_received + 1);  
            strcpy(final_buffer, peer->incoming_buffer);
            strcat(final_buffer, *buffer);
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


int read_messages(int bytes_received, char *msg_buffer, void * p, int (*callback)(int, char *, void *)) {
    char *context;
    char *msg = strtok_r(msg_buffer, "|", &context);

    for (; msg; msg = strtok_r(NULL, "|", &context)) {
        if (callback(bytes_received, msg, p)) {
            return 1;
        }
    }

    return 0;
}

int send_message(int socket, const char *message, int flags) {
    size_t message_length = strlen(message);
    size_t bytes_sent = 0;
    while (bytes_sent < message_length) {
        ssize_t result = send(socket, message + bytes_sent, min(512, message_length), flags);
        if (result == -1) {
            perror("Send failed");
            return -1; // Error occurred
        }
        bytes_sent += result;
    }
    return 0; // Success
}