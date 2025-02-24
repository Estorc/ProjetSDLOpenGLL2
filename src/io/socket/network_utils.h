#pragma once

#ifdef _WIN32
#define MSG_NOSIGNAL 0
#endif

#define MSG_TERMINATOR '|'

#pragma region LOW LEVEL
int initiate_socket();
void close_socket(int sock);
struct socket_request_listener create_socket_request_listener(int sock);
int socket_request_listen(struct socket_request_listener * listener, int server_sock, int timeout);
int socket_request_receive(struct socket_request_listener * listener, int client_sock, char * buffer, int size, int timeout, int flags);
#pragma region HIGH LEVEL
int receive_message(void *peer, char **buffer, int size, int timeout, int flags);
int read_messages(int bytes_received, char *msg_buffer, void * p, int (*callback)(int, char *, void *));