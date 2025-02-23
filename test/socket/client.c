#include "../src/io/socket/socket.h"
#include "../src/raptiquax.h"
#include <pthread.h>

#define PORT 30000
#define MAX_PING 1000000
char buffer[512];
char buffer_stack[512][512];
char *incoming_buffer = NULL;
int buffer_stack_index = 0;
int quit = 0;
int ping = 0;


static int get_message(struct socket_request_listener *request_listener, int sock, char **buffer, int size, int timeout) {
    free(*buffer);
    *buffer = malloc(sizeof(char) * size);
    int lg = socket_request_receive(request_listener, sock, *buffer, size, timeout, 0);
    if (lg != -1 && incoming_buffer && *incoming_buffer) {
        int len = strlen(incoming_buffer) + 1 + size;
        char *final_buffer = malloc(sizeof(char) * len);  
        strcpy(final_buffer, incoming_buffer);
        strcat(final_buffer, *buffer);
        free(*buffer);
        *buffer = final_buffer;
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
        int lg = get_message(&request_listener, server_sock, &msg_buffer, 512, 1000);
        if (lg == -1) {
            ping++;
            if (ping > MAX_PING) {
                PRINT_CLIENT_INFO("Ping timeout\n");
                quit = 1;
            }
        }
        while (lg != -1) {
            ping = 0;
            bool incomplete_message = (msg_buffer[strlen(msg_buffer) - 1] != '|');
            char * msg = strtok(msg_buffer, "|");
            char * next_msg;
            while (msg) {
                next_msg = strtok(NULL, "|");
                if (!next_msg && incomplete_message) {
                    break;
                }


                if (lg == 0) {
                    PRINT_INFO("Server disconnected\n");
                    quit = 1;
                    break;
                }
                if (strcmp(msg, "PING") == 0) {
                    if (buffer_stack_index) {
                        strcpy(buffer, buffer_stack[buffer_stack_index - 1]);
                        buffer_stack_index--;
                        int len = strlen(buffer) + 1;
                        buffer[len - 1] = '|';
                        if (buffer[0] == '$') {
                            send(server_sock, "It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).|", 614, 0);
                        } else {
                            send(server_sock, buffer, len, 0);
                        }
                    } else {
                        send(server_sock, "PONG|", 5, 0);
                    }
                } else {
                    PRINT_INFO("Received %d bytes\n", lg);
                    PRINT_SERVER_INFO("%s\n", msg);
                }

                msg = next_msg;
            }
            incoming_buffer = strdup(msg);
            lg = get_message(&request_listener, server_sock, &msg_buffer, 512, 1000);
        }
    }
    free(msg_buffer);
    return NULL;
}

void *output_server(void *arg) {
    while (!quit) {
        fgets(buffer, 511, stdin);
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