#include "../src/io/socket/socket.h"
#include "../src/raptiquax.h"
#include <pthread.h>

#define PORT 30000
char buffer[512];
char buffer_stack[512][512];
char *incoming_buffer;
int buffer_stack_index = 0;
int quit = 0;
int ping = 0;


static int get_message(struct socket_request_listener *request_listener, int sock, char **buffer, int size, int timeout) {
    free(*buffer);
    *buffer = malloc(sizeof(char) * size);
    int lg = socket_request_receive(request_listener, sock, *buffer, size, timeout);
    if (lg != -1 && incoming_buffer && *incoming_buffer) {
        int len = strlen(incoming_buffer) + 1 + size;
        *buffer = realloc(*buffer, sizeof(char) * len);  
        strcat(*buffer, incoming_buffer);
        free(incoming_buffer);
        incoming_buffer = NULL;
        return len;
    }
    return lg;
}

void *input_server(void *arg) {
    int server_sock = *(int *)arg;
    char * msg_buffer = NULL;
    struct socket_request_listener request_listener = create_socket_request_listener(server_sock);
    while (!quit) {
        int lg = get_message(&request_listener, server_sock, &msg_buffer, 512, 1);
        if (lg == -1) {
            ping++;
            if (ping > 100) {
                PRINT_CLIENT_INFO("Ping timeout\n");
                quit = 1;
            }
        }
        while (lg != -1) {
            ping = 0;
            char * msg = strtok(msg_buffer, "|");
            for (;msg;msg = strtok(NULL, "|")) {
                if (!strchr(msg, '|'))
                    break;
                if (lg == 0) {
                    PRINT_INFO("Server disconnected\n");
                    quit = 1;
                    break;
                }
                if (strcmp(msg, "PING") == 0) {
                    if (buffer_stack_index) {
                        strcpy(buffer, buffer_stack[buffer_stack_index - 1]);
                        buffer_stack_index--;
                        send(server_sock, buffer, strlen(buffer), 0);
                    } else
                        send(server_sock, "PONG", 5, 0);
                } else {
                    PRINT_INFO("Received %d bytes\n", lg);
                    PRINT_SERVER_INFO("%s\n", msg);
                }
            }
            incoming_buffer = strdup(msg);
            lg = get_message(&request_listener, server_sock, &msg_buffer, 512, 1);
        }
    }
    free(msg_buffer);
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

    printf("Enter server name: ");
    char SERVER_NAME[256] = {0};
    scanf("%s", SERVER_NAME);
    getc(stdin);
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