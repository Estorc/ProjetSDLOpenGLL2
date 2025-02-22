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
    #include <unistd.h>
    #define CLOSESOCKET close
#endif

struct socket_request_listener {
    fd_set readfds;
    struct timeval timeout;
};

#include "server.h"
#include "client.h"
#include "network_utils.h"