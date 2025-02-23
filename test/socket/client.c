#include "../src/io/socket/socket.h"
#include "../src/raptiquax.h"
#include <pthread.h>

#define SERVER_NAME "pluscorp.fr"
#define PORT 30000
char buffer[512];
char buffer_stack[512][512];
int buffer_stack_index = 0;
int quit = 0;
int ping = 0;

void *input_server(void *arg) {
    int server_sock = *(int *)arg;
    struct socket_request_listener request_listener = create_socket_request_listener(server_sock);
    while (!quit) {
        int lg = socket_request_receive(&request_listener, server_sock, buffer, 512, 1);
        if (lg != -1) {
            ping = 0;
            if (lg == 0) {
                PRINT_INFO("Server disconnected\n");
                quit = 1;
                break;
            }
            if (strcmp(buffer, "PING") == 0) {
                if (buffer_stack_index) {
                    strcpy(buffer, buffer_stack[buffer_stack_index - 1]);
                    buffer_stack_index--;
                    send(server_sock, buffer, strlen(buffer), 0);
                } else
                    send(server_sock, "PONG", 5, 0);
            } else {
                PRINT_INFO("Received %d bytes\n", lg);
                PRINT_SERVER_INFO("%s\n", buffer);
            }
        } else {
            ping++;
            if (ping > 100) {
                PRINT_CLIENT_INFO("Ping timeout\n");
                quit = 1;
            }
        }
    }
    return NULL;
}

void *output_server(void *arg) {
    while (!quit) {
        fgets(buffer, 512, stdin);
        strcpy(buffer_stack[buffer_stack_index], buffer);
        buffer_stack_index++;
    }
    return NULL;
}

int main(int argc, char **argv) {
    initiate_socket();

    int server_sock = connect_socket_server(SERVER_NAME, PORT);
    PRINT_CLIENT_INFO("Connected to server!\n");

    /* envoie de données et reception */

    pthread_t input_thread, ping_thread;

    pthread_create(&ping_thread, NULL, input_server, &server_sock);
    pthread_create(&input_thread, NULL, output_server, &server_sock);

    pthread_join(input_thread, NULL);
    pthread_join(ping_thread, NULL);

    close_socket(server_sock);

    return 0;
}