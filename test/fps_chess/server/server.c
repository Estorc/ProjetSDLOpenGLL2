#include "../../../src/io/socket/socket.h"
#include "../../../src/storage/hash.h"
#include "../../../src/raptiquax.h"
#include <errno.h>

struct client_info {
    char *name;
    char *avatar_raw_data;
};

struct party {
    char *name;
    struct client **clients;
    HashTable *data;
};

struct client {
    int socket;
    struct socket_request_listener listener;
    char *incoming_buffer;
    
    bool authorized;
    int ping;
    struct party *party;
    struct client_info info;
};

static int MAX_CLIENTS;
static int MAX_PARTY_CLIENTS;
static int PORT;
static int MAX_PING;
static int TIMEOUT;
static char * SERVER_NAME;
static char * PASSWORD;


const char default_config[] = 
"#--------------------------------------------------\n"
"# Server Configuration File\n"
"#--------------------------------------------------\n"
"\n"
"# Server Configuration\n"
"server-name=FPS Chess Server\n"
"max-clients=20\n"
"max-party-clients=2\n"
"port=30000\n"
"password=lobby\n"
"max-ping=100\n"
"timeout=8000\n";

struct client *clients;
char buffer[512];

static void kill_client(struct client *client);

static void load_config() {
    FILE * config = fopen("server-config.cfg", "r");
    if (config == NULL) {
        config = fopen("server-config.cfg", "w");
        fprintf(config, "%s", default_config);
        fclose(config);
        config = fopen("server-config.cfg", "r");
        if (config == NULL) {
            PRINT_ERROR("Error creating default config file!\n");
            exit(1);
        }
        PRINT_INFO("Default config file created!\n");
    }
    char *config_content = read_filef(config);
    fclose(config);
    PRINT_INFO("Config file loaded!\n");

    char *context;
    char *line = strtok_s(config_content, "\n", &context);
    for (; line; line = strtok_s(NULL, "\n", &context)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");
        if (strcmp(key, "max-clients") == 0) {
            MAX_CLIENTS = atoi(value);
        } else if (strcmp(key, "max-party-clients") == 0) {
            MAX_PARTY_CLIENTS = atoi(value);
        } else if (strcmp(key, "port") == 0) {
            PORT = atoi(value);
        } else if (strcmp(key, "server-name") == 0) {
            SERVER_NAME = strdup(value);
        } else if (strcmp(key, "password") == 0) {
            PASSWORD = strdup(value);
        } else if (strcmp(key, "max-ping") == 0) {
            MAX_PING = atoi(value);
        } else if (strcmp(key, "timeout") == 0) {
            TIMEOUT = atoi(value);
        }
    }
    free(config_content);

    PRINT_INFO("Config loaded!\n");
}


static inline void send_message_with_separator(struct client *client, const char *message) {
    int len = strlen(message) + 1;
    char *buffer = malloc(sizeof(char) * len);
    strcpy(buffer, message);
    buffer[len-1] = '|';
    int bytes_sent = send_message(client->socket, buffer, MSG_NOSIGNAL);
    if (bytes_sent == -1) {
        if (errno == EPIPE || errno == ECONNRESET) {
            PRINT_ERROR("Connection lost, closing socket\n");
            kill_client(client);
        } else {
            perror("send failed");
            PRINT_ERROR("Failed to send message\n");
        }
    }
    free(buffer);
}


static inline bool command(const char *cmd, char *str, char **args) {
    int len = strlen(cmd);
    if (strncmp(cmd, str, len) == 0) {
        str[strlen(str)] = 0;
        *args = str + len + 1;
        return true;
    }
    return false;
}


static inline bool is_first_client_of_party(struct client *client, struct party *party) {
    return party->clients[0] == client;
}


static inline int get_client_id(struct client *client) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == client->socket) {
            return i;
        }
    }
    return -1;
}


static inline int get_party_id(struct party *party) {
    return get_client_id(party->clients[0]);
}


static char * list_party() {
    int party_list_size = 1<<4;
    char *party_list = malloc(party_list_size);
    char field[256];
    party_list[0] = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket && clients[i].party && is_first_client_of_party(&clients[i], clients[i].party)) {
            sprintf(field, "\nParty %d - %s\n", i, clients[i].party->name);
            if (strlen(party_list) + strlen(field) + 1 > party_list_size) {
                party_list_size <<= 1;
                party_list = realloc(party_list, party_list_size);
            }
            strcat(party_list, field);
            for (int j = 0; j < MAX_PARTY_CLIENTS; j++) {
                if (clients[i].party->clients[j]) {
                    sprintf(field, "Client %d : %s\n", j, clients[i].party->clients[j]->info.name);
                    if (strlen(party_list) + strlen(field) + 1 > party_list_size) {
                        party_list_size <<= 1;
                        party_list = realloc(party_list, party_list_size);
                    }
                    strcat(party_list, field);
                }
            }
        }
    }
    PRINT_SERVER_INFO("Party list : %s\n", party_list);
    return party_list;
}

static inline void destroy_party(struct party *party) {
    PRINT_SERVER_INFO("Destroying party %s\n", party->name);
    free(party->clients);
    free(party->name);
    table_free(party->data);
    free(party);
}

static void exit_party(struct client *client) {
    if (client->party) {
        PRINT_SERVER_INFO("%s exiting party %s\n", client->info.name, client->party->name);
        bool destroy = true;
        int shift = 0;
        for (int i = 0; i < MAX_PARTY_CLIENTS; i++) {
            if (client->party->clients[i] == client) {
                PRINT_SERVER_INFO("Removing client %d from party %s\n", i, client->party->name);
                client->party->clients[i] = NULL;
                shift++;
            } else if (client->party->clients[i]) {
                if (shift) {
                    client->party->clients[i - shift] = client->party->clients[i];
                    client->party->clients[i] = NULL;
                }
                destroy = false;
            }
        }
        if (destroy) {
            destroy_party(client->party);
        }
        client->party = NULL;
    }
}

static void join_party(struct client *client, int party_id) {
    struct party *party = clients[party_id].party;
    if (!party) {
        PRINT_SERVER_INFO("Party %d not found!\n", party_id);
        return;
    }
    if (client->party) {
        PRINT_SERVER_INFO("Already on a party!\n");
        return;
    }
    PRINT_SERVER_INFO("Joining party %s\n", party->name);
    for (int i = 0; i < MAX_PARTY_CLIENTS; i++) {
        if (party->clients[i] == NULL) {
            party->clients[i] = client;
            client->party = party;
            return;
        }
    }
    if (!client->party) {
        PRINT_SERVER_INFO("Party full!\n");
    }
}

static void create_party(struct client *client, const char *name) {
    PRINT_SERVER_INFO("Creating party %s\n", name);
    struct party *party = malloc(sizeof(struct party));
    party->name = strdup(name);
    party->clients = malloc(sizeof(struct client *) * MAX_PARTY_CLIENTS);
    party->clients[0] = client;
    party->data = table_create(16);
    memset(party->clients + 1, 0, sizeof(struct client *) * (MAX_PARTY_CLIENTS - 1));
    if (client->party) {
        exit_party(client);
    }
    client->party = party;
}


static inline void init_client(struct client *client, int socket) {
    PRINT_SERVER_INFO("Initializing client\n");
    client->socket = socket;
    client->authorized = false;
    client->listener = create_socket_request_listener(socket);
    client->info.name = strdup("Anonymous");
}


static void kill_client(struct client *client) {
    PRINT_SERVER_INFO("Killing client\n");
    if (client->party) {
        exit_party(client);
    }
    if (client->socket) {
        shutdown(client->socket, 2);
        CLOSESOCKET(client->socket);
        client->socket = 0;
    }
    if (client->info.name) {
        free(client->info.name);
    }
    if (client->incoming_buffer) {
        free(client->incoming_buffer);
    }
    memset(client, 0, sizeof(*client));
}




int handle_message(int bytes_received, char *msg, void * p) {
    struct client *client = (struct client *)p;
    char *args;
    if (command("PONG", msg, &args));
    else if (client->authorized) {
        PRINT_INFO("Received %d bytes\n", bytes_received);
        if (command("LOGIN", msg, &args)) {
            PRINT_SERVER_INFO("Client login : %s\n", args);
            free(client->info.name);
            client->info.name = strdup(args);
            send_message_with_separator(client, "Logged in!");
        } else if (command("CREATE_PARTY", msg, &args)) {
            create_party(client, args);
            sprintf(buffer, "Party %s created!", args);
            send_message_with_separator(client, buffer);
        } else if (command("LIST_PARTY", msg, &args)) {
            char * party_list = list_party();
            if (party_list[0] == 0) {
                send_message_with_separator(client, "No party available!");
            } else {
                send_message_with_separator(client, party_list);
            }
            free(party_list);
        } else if (command("EXIT_PARTY", msg, &args)) {
            if (client->party) {
                exit_party(client);
                send_message_with_separator(client, "Exited party!");
            } else {
                send_message_with_separator(client, "No party to exit!");
            }
        } else if (command("JOIN_PARTY", msg, &args)) {
            join_party(client, atoi(args));
            if (client->party) {
                send_message_with_separator(client, "Joined party!");
            } else {
                send_message_with_separator(client, "Failed to join party!");
            }
        } else if (command("RENAME_PARTY", msg, &args)) {
            if (client->party) {
                free(client->party->name);
                client->party->name = strdup(args);
                sprintf(buffer, "Party renamed to %s", args);
                send_message_with_separator(client, buffer);
            } else {
                send_message_with_separator(client, "No party to rename!");
            }
        } else if (command("PARTY_SET_DATA", msg, &args)) {
            if (client->party) {
                char *key = strtok(args, "=");
                char *value = strtok(NULL, "=");
                table_insert_raw(client->party->data, key, strdup(value));
                send_message_with_separator(client, "Data set!");
            } else {
                send_message_with_separator(client, "No party to set data!");
            }
        } else if (command("PARTY_GET_DATA", msg, &args)) {
            if (client->party) {
                char *value = table_get(client->party->data, args);
                if (value) {
                    send_message_with_separator(client, value);
                } else {
                    send_message_with_separator(client, "Data not found!");
                }
            } else {
                send_message_with_separator(client, "No party to get data!");
            }
        } else if (bytes_received == 0) {
            PRINT_CLIENT_INFO("deconnected\n");
            kill_client(client);
        } else if (command("G_MSG", msg, &args)) {
            msg[bytes_received-1] = 0;
            PRINT_SERVER_INFO("%s send message : '%s'\n", client->info.name, msg + 6);
            char * message = malloc(sizeof(char) * (strlen(msg + 6) + 1 + strlen(client->info.name) + 3));
            sprintf(message, "%s : %s", client->info.name, msg + 6);
            for (int j = 0; j < MAX_CLIENTS; j++) {
                if (clients[j].socket && clients[j].authorized) {
                    send_message_with_separator(&clients[j], message);
                }
            }
            free(message);
        } else {
            msg[bytes_received-1] = 0;
            PRINT_SERVER_INFO("%s send message : '%s'\n", client->info.name, msg);
            char * message = malloc(sizeof(char) * (strlen(msg) + 1 + strlen(client->info.name) + 3));
            sprintf(message, "%s : %s", client->info.name, msg);
            if (client->party) {
                for (int j = 0; j < MAX_PARTY_CLIENTS; j++) {
                    if (client->party->clients[j]) {
                        send_message_with_separator(client->party->clients[j], message);
                    }
                }
            } else {
                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].socket && clients[j].authorized) {
                        send_message_with_separator(&clients[j], message);
                    }
                }
            }
            free(message);
        }
    } else {
        if (strncmp(PASSWORD, msg, strlen(PASSWORD)) == 0) {
            PRINT_SERVER_INFO("Client authorized!\n");
            client->authorized = true;
            send_message_with_separator(client, "AUTHORIZED");
        } else {
            PRINT_SERVER_INFO("Client not authorized!\n");
            send_message_with_separator(client, "NOT AUTHORIZED");
            kill_client(client);
        }
    }
    return 0;
}



int main(int argc, char **argv) {
    update_cwd();
    load_config();

    PRINT_SERVER_INFO("Server name: %s\n", SERVER_NAME);
    PRINT_SERVER_INFO("Max clients: %d\n", MAX_CLIENTS);
    PRINT_SERVER_INFO("Port: %d\n", PORT);
    PRINT_SERVER_INFO("Password: %s\n", PASSWORD);
    PRINT_SERVER_INFO("Max ping: %d\n", MAX_PING);
    PRINT_SERVER_INFO("Max party clients: %d\n", MAX_PARTY_CLIENTS);
    PRINT_SERVER_INFO("Timeout: %d\n", TIMEOUT);


    clients = malloc(sizeof(struct client) * MAX_CLIENTS);

    memset(clients, 0, sizeof(*clients) * MAX_CLIENTS);

    initiate_socket();
    int server_sock = create_socket_server(PORT);
    PRINT_SERVER_INFO("Server started!\n");
    listen_socket(server_sock);
    struct socket_request_listener request_listener = create_socket_request_listener(server_sock);
    PRINT_SERVER_INFO("In listening!\n");

    int exit = 0;
    while (!exit) {
        int client_sock = socket_request_listen(&request_listener, server_sock, TIMEOUT);
        if (client_sock) {
            PRINT_SERVER_INFO("Received connexion!\n");
            for (int i = 0; i <= MAX_CLIENTS; i++) {
                if (i == MAX_CLIENTS) {
                    PRINT_SERVER_INFO("Too many clients!\n");
                    break;
                }
                if (clients[i].socket == 0) {
                    PRINT_SERVER_INFO("Adding client to list!\n");
                    init_client(&clients[i], client_sock);
                    if (strlen(PASSWORD) == 0) {
                        PRINT_SERVER_INFO("No password required!\n");
                        clients[i].authorized = true;
                    } else {
                        PRINT_SERVER_INFO("Password required!\n");
                    }
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_CLIENTS; i++) {
            struct client *client = &clients[i];
            if (client->socket == 0) {
                continue;
            }

            char *msg_buffer = NULL;
            int bytes_received = receive_message(client, &msg_buffer, 512, TIMEOUT, 0);
            if (bytes_received == -1) {
                if (++client->ping > MAX_PING) {
                    PRINT_CLIENT_INFO("Ping timeout\n");
                    kill_client(client);
                } else {
                    send_message_with_separator(client, "PING");
                }
            }
            if (bytes_received != -1 && client->socket) {
                client->ping = 0;
                read_messages(bytes_received, msg_buffer, client, handle_message);
            }
        }
    }

    close_socket(server_sock);

    free(SERVER_NAME);
    free(PASSWORD);
    free(clients);
    return 0;
}
