#include "../../raptiquax.h"
#include "socket.h"

struct sockaddr_in get_address(const char * name, int port) {
    struct sockaddr_in server_addr;
    struct hostent *serveur_info;
    long hostAddr;
    memset(&server_addr, 0, sizeof(server_addr));
    hostAddr = inet_addr(name);
    if ((long)hostAddr != (long)-1) {
        memcpy(&server_addr.sin_addr, &hostAddr, sizeof(hostAddr));
    } else {
        serveur_info = gethostbyname(name);
        if (serveur_info == NULL) {
            PRINT_ERROR("gethost rate\n");
            exit(0);
        }
        memcpy(&server_addr.sin_addr, serveur_info->h_addr, serveur_info->h_length);
    }
    server_addr.sin_port = htons(30000);
    server_addr.sin_family = AF_INET;

    return server_addr;
}


int connect_socket_server(const char * name, int port) {
    struct sockaddr_in server_addr = get_address(name, port);

    int sock;
    /* création de la socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        PRINT_ERROR("Unable to create the client socket\n");
        exit(0);
    }

    /* requete de connexion */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        PRINT_ERROR("Unable to reach the server\n");
        exit(0);
    }
    return sock;
}