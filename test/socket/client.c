#include "../src/io/socket/socket.h"
#include "../src/raptiquax.h"

#define SERVEURNAME "127.0.0.1"
char buffer[512];

int main(int argc, char **argv) {
    initiate_socket();

    int server_sock = connect_socket_server(SERVEURNAME, 30000);
    PRINT_CLIENT_INFO("Connected to server!\n");

    /* envoie de données et reception */

    struct socket_request_listener request_listener = create_socket_request_listener(server_sock);
    while (1) {
        printf("Enter a message : ");
        fgets(buffer, 512, stdin);
        send(server_sock, buffer, strlen(buffer), 0);
        int lg = socket_request_receive(&request_listener, server_sock, buffer, 512, 50000);
        if (lg == -1) {
            PRINT_ERROR("Error while receiving data\n");
            break;
        } else {
            PRINT_INFO("Received %d bytes\n", lg);
            PRINT_SERVER_INFO("Server response : %s\n", buffer);
        }
        printf("Do you want to continue ? (y/n) : ");
        char choix;
        scanf("%c", &choix);
        if (choix == 'n') {
            break;
        }
        getchar();
    }

    close_socket(server_sock);

    return 0;
}