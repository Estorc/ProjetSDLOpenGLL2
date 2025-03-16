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

/**
 * @struct socket_request_listener
 * @brief Structure to hold socket request listener information.
 *
 * This structure contains the file descriptor set and timeout value
 * used for monitoring socket read events.
 *
 * @var socket_request_listener::readfds
 * File descriptor set used for monitoring read events on sockets.
 *
 * @var socket_request_listener::timeout
 * Timeout value for the select() system call, specifying the maximum
 * time to wait for an event.
 */
struct socket_request_listener {
    fd_set readfds;
    struct timeval timeout;
};

/**
 * @struct peer
 * @brief Structure to represent a network peer.
 *
 * This structure contains information about a network peer, including
 * its socket, request listener, incoming buffer, ping value, and message
 * handlers.
 *
 * @var peer::socket
 * The socket file descriptor associated with the peer.
 *
 * @var peer::listener
 * The socket request listener associated with the peer, used to monitor
 * read events and handle timeouts.
 *
 * @var peer::incoming_buffer
 * Pointer to a buffer used for storing incoming data from the peer.
 *
 * @var peer::ping
 * An integer value representing the ping (latency) of the peer.
 *
 * @var peer::handlers
 * Array of pointers to message handler structures, used to process
 * incoming messages from the peer.
 *
 * @var peer::handler_count
 * The number of message handlers in the handlers array.
 */
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
    int (*callback)(int msglen, char *msg, void * p, void ** data);
    int lifespan;
    void ** data;
};

#include "server.h"
#include "client.h"
#include "network_utils.h"