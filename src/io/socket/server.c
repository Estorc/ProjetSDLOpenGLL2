#include "../../raptiquax.h"
#include "socket.h"

struct sockaddr_in create_address(int port) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    return server_addr;
}

int create_socket_server(int port) {
    struct sockaddr_in server_addr = create_address(port);

    int sock;
    /* Création de la socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        PRINT_ERROR("Unable to create the server socket\n");
        exit(0);
    }

    /* Bind serveur - socket */
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        PRINT_ERROR("Bind error\n");
        CLOSESOCKET(sock);
        exit(0);
    }
    return sock;
}

int listen_socket(int sock) {
    if (listen(sock, 5) == -1) {
        PRINT_ERROR("Listen error\n");
        CLOSESOCKET(sock);
        exit(0);
    }
    return 0;
}