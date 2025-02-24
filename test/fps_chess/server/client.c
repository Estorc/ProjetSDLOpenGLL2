#include "../src/io/socket/socket.h"
#include "../src/raptiquax.h"
#include <pthread.h>

#define PORT 30000
#define MAX_PING 1000000

int quit = 0;
int ping = 0;


int handle_message(int bytes_received, char *msg, void * p) {
    struct peer *peer = (struct peer *)p;
    if (strcmp(msg, "PING") == 0) {
        send(peer->socket, "PONG|", 5, 0);
    } else {
        PRINT_INFO("Received %d bytes\n", bytes_received);
        PRINT_SERVER_INFO("%s\n", msg);
    }
    return 0;
}


void *input_server(void *arg) {
    struct peer *peer = (struct peer *)arg;
    char *msg_buffer = NULL;

    while (!quit) {
        int bytes_received = receive_message(peer, &msg_buffer, 512, 1000, 0);

        if (bytes_received == -1) {
            if (++ping > MAX_PING) {
                PRINT_CLIENT_INFO("Ping timeout\n");
                quit = 1;
            }
            continue;
        }

        ping = 0;
        if (bytes_received == 0) {
            PRINT_INFO("Server disconnected\n");
            quit = 1;
            break;
        }

        read_messages(bytes_received, msg_buffer, peer, handle_message);

    }

    free(msg_buffer);
    return NULL;
}

void *output_server(void *arg) {
    struct peer *peer = (struct peer *)arg;
    while (!quit) {
        char buffer[512];
        fgets(buffer, 511, stdin);
        int len = strlen(buffer) + 1;
        buffer[len - 2] = '|';
        buffer[len - 1] = 0;
        if (buffer[0] == '$') {
            send(peer->socket, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris id erat sem. In hac habitasse platea dictumst. Vivamus eu tempus ligula. Praesent odio neque, sodales quis enim at, porttitor aliquet nisi. Donec eleifend nulla nec sapien luctus, pulvinar faucibus libero efficitur. Aenean ac metus sed libero finibus ultricies. Proin in dui sit amet ligula venenatis rutrum. Vivamus nec nisl tortor. Nunc aliquet convallis ligula, quis aliquam mauris mattis id. Pellentesque dapibus vitae erat nec sollicitudin. Nullam volutpat rutrum elit, aliquet sodales ante lobortis vitae. Aenean sodales, elit vitae hendrerit gravida, enim dui aliquet libero, eu feugiat tellus leo tincidunt dolor. Sed tristique sit amet magna interdum pellentesque. Donec fermentum consectetur bibendum. Maecenas id risus quis ante dignissim suscipit. Quisque volutpat leo vitae elit hendrerit rutrum. Etiam consequat, mauris at condimentum mollis, libero magna ultrices nisi, ut eleifend quam sapien sed metus. Nullam gravida tempus varius. Vivamus efficitur tempus euismod. Mauris ut dolor et justo bibendum imperdiet. Nulla viverra ex nunc, vel sagittis nulla sollicitudin ut. Aliquam sagittis venenatis volutpat. Aenean tortor lectus, rhoncus sed mauris sit amet, rutrum cursus sapien. Pellentesque in velit nec turpis hendrerit luctus id quis justo. Donec quam ipsum, posuere interdum mattis ut, laoreet non est. Proin quis accumsan ante. Sed laoreet, sem at mollis vulputate, nisi est dignissim orci, quis pellentesque odio nisl in mi. Vivamus justo sem, ultricies vel fringilla et, aliquam et nibh. Vivamus non sagittis purus. Ut semper eu sem nec ornare. Aliquam nec tincidunt est. Donec ornare finibus mi, sit amet pellentesque lacus ullamcorper a. Cras ut dignissim ipsum, dignissim blandit justo. Morbi egestas augue non ante tincidunt placerat. Curabitur at porta tellus. Nam laoreet nunc quis felis mollis aliquet. Aenean aliquet, arcu in facilisis suscipit, orci quam condimentum mi, a commodo nisl elit dictum tortor. Aliquam vitae laoreet sem, eget maximus est. Donec consectetur ipsum eget orci varius, quis sodales turpis laoreet. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Quisque scelerisque turpis pharetra lectus accumsan, at ultricies arcu fermentum. In nec orci mi. Duis eu elit maximus, porttitor justo non, vulputate diam. Quisque ipsum massa, posuere vel lorem vitae, dignissim blandit erat. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nulla facilisi. Curabitur tincidunt massa interdum sollicitudin lobortis. Aenean malesuada ex vitae risus dignissim interdum. Aliquam aliquam pharetra erat id ultricies. Donec vel mollis ipsum. Aliquam erat volutpat. Nunc mattis arcu lorem, non porttitor libero malesuada nec. Sed suscipit rutrum velit. In lectus tortor, tincidunt nec est sed, iaculis condimentum justo. Donec pretium tortor vitae lorem tincidunt finibus. Sed non dui ac magna rutrum eleifend. Interdum et malesuada fames ac ante ipsum primis in faucibus.|", 3054, 0);
        } else {
            send(peer->socket, buffer, len, 0);
        }
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

    struct peer peer = {server_sock, create_socket_request_listener(server_sock), NULL};

    pthread_create(&ping_thread, NULL, input_server, &peer);
    pthread_create(&input_thread, NULL, output_server, &peer);

    pthread_detach(input_thread);
    pthread_detach(ping_thread);

    pthread_join(input_thread, NULL);
    pthread_join(ping_thread, NULL);

    printf("test\n");

    while(!quit);

    close_socket(server_sock);

    return 0;
}