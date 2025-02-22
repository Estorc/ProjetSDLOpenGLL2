#include "../src/io/socket/socket.h"
#include "../src/raptiquax.h"

#define MAX_CLIENTS 20

int clients[MAX_CLIENTS];
struct socket_request_listener clients_listener[MAX_CLIENTS];
char buffer[512];

int main(int argc, char **argv) {
    memset(clients, 0, sizeof(clients));

    initiate_socket();

    int server_sock = create_socket_server(30000);
    PRINT_SERVER_INFO("Server started!\n");

    listen_socket(server_sock);
    struct socket_request_listener request_listener = create_socket_request_listener(server_sock);
    PRINT_SERVER_INFO("In listening!\n");

    int quitter = 0;
    while (!quitter) {
        int client = socket_request_listen(&request_listener, server_sock, 5000);
        if (client) {
            PRINT_SERVER_INFO("Received connexion!\n");
            for (int i = 0; i <= MAX_CLIENTS; i++) {
                if (i == MAX_CLIENTS) {
                    PRINT_SERVER_INFO("Too many clients!\n");
                    break;
                }
                if (clients[i] == 0) {
                    PRINT_SERVER_INFO("Adding client to list!\n");
                    clients[i] = client;
                    clients_listener[i] = create_socket_request_listener(client);
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int client = clients[i];
            if (client == 0) {
                continue;
            }
            int lg = socket_request_receive(&clients_listener[i], client, buffer, 512, 5000);
            if (lg != -1) {
                PRINT_INFO("Received %d bytes\n", lg);
                if (strncmp("MSG", buffer, 3) == 0) {
                    PRINT_SERVER_INFO("got message : '%s'\n", buffer + 4);
                    PRINT_SERVER_INFO("send answer!\n");
                    strcpy(buffer, "REPONSE DU SERVEUR");
                    send(client, buffer, 512, 0);
                } else if (strncmp("QUITTER", buffer, 7) == 0) {
                    PRINT_CLIENT_INFO("asked for deconnecting : '%s'\n", buffer);
                    shutdown(client, 2);
                    CLOSESOCKET(client);
                    quitter = 1;
                } else if (strncmp("BONJOUR", buffer, 7) == 0) {
                    PRINT_CLIENT_INFO("%7s\n", buffer);
                    send(client, "BONJOUR", 7, 0);
                } else if (lg == 0) {
                    PRINT_CLIENT_INFO("deconnected\n");
                    CLOSESOCKET(client);
                    clients[i] = 0;
                }
            }
        }
    }

    close_socket(server_sock);

    return 0;
}
