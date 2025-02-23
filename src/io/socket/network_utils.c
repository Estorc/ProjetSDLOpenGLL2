#include "../../raptiquax.h"
#include "socket.h"

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