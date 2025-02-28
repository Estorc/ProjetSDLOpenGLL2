#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library
#define CLOSESOCKET closesocket  // Windows uses closesocket() instead of close()

#else
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define CLOSESOCKET close
#endif

struct socket_request_listener {
    fd_set readfds;
    struct timeval timeout;
};

struct peer {
    int socket;
    struct socket_request_listener listener;
    char *incoming_buffer;
    int ping;
    struct message_handler **handlers;
    int handler_count;
};

/**
 * @struct message_handler
 * @brief Structure to handle messages
 * 
 * @param msg The message to handle
 * @param callback The callback to call when the message is received
 * @param lifespan The number of time the message can be received
 * @param data The data to pass to the callback
 * 
 */
struct message_handler {
    char *msg;
    int (*callback)(int bytes_received, char *msg, void * p, void ** data);
    int lifespan;
    void ** data;
};

#include "server.h"
#include "client.h"
#include "network_utils.h"