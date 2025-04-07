#pragma once
/**
 * @file network_utils.h
 * @brief Network utility functions and structures for socket communication.
 */

 
#ifdef _WIN32
#define MSG_NOSIGNAL 0
#endif

#define MSG_TERMINATOR '|'

#pragma region LOW LEVEL


/**
 * @brief Initiates a socket.
 * 
 * @return int The file descriptor of the created socket, or -1 on error.
 */
int initiate_socket();

/**
 * @brief Closes a socket.
 * 
 * @param sock The file descriptor of the socket to close.
 */
void close_socket(int sock);

/**
 * @brief Creates a socket request listener.
 * 
 * @param sock The file descriptor of the socket to listen on.
 * @return struct socket_request_listener The created socket request listener.
 */
struct socket_request_listener create_socket_request_listener(int sock);

/**
 * @brief Listens for socket requests.
 * 
 * @param listener Pointer to the socket request listener.
 * @param server_sock The file descriptor of the server socket.
 * @param timeout The timeout value for listening, in milliseconds.
 * @return int 0 on success, or -1 on error.
 */
int socket_request_listen(struct socket_request_listener * listener, int server_sock, int timeout);

/**
 * @brief Receives a socket request.
 * 
 * @param listener Pointer to the socket request listener.
 * @param client_sock The file descriptor of the client socket.
 * @param buffer Pointer to the buffer to store the received data.
 * @param size The size of the buffer.
 * @param timeout The timeout value for receiving, in milliseconds.
 * @param flags Flags for receiving data.
 * @return int The number of bytes received, or -1 on error.
 */
int socket_request_receive(struct socket_request_listener * listener, int client_sock, char * buffer, int size, int timeout, int flags);

#pragma region HIGH LEVEL

/**
 * @brief Creates a peer structure.
 * 
 * @param socket The file descriptor of the socket associated with the peer.
 * @return struct peer* Pointer to the created peer structure.
 */
struct peer *create_peer(int socket);

/**
 * @brief Receives a message from a peer.
 * 
 * @param peer Pointer to the peer structure.
 * @param buffer Pointer to the buffer to store the received message.
 * @param size The size of the buffer.
 * @param timeout The timeout value for receiving, in milliseconds.
 * @param flags Flags for receiving data.
 * @return int The number of bytes received, or -1 on error.
 */
int receive_message(void *peer, char **buffer, int size, int timeout, int flags);

/**
 * @brief Reads messages from a buffer.
 * 
 * @param bytes_received The number of bytes received.
 * @param msg_buffer Pointer to the buffer containing the messages.
 * @param p Pointer to additional data.
 * @return int 0 on success, or -1 on error.
 */
int read_messages(int bytes_received, char *msg_buffer, void * p);

/**
 * @brief Sends a message through a socket.
 * 
 * @param socket The file descriptor of the socket.
 * @param message Pointer to the message to send.
 * @param flags Flags for sending data.
 * @return int The number of bytes sent, or -1 on error.
 */
int send_message(int socket, const char *message, int flags);

/**
 * @brief Removes a message handler by its index.
 * 
 * @param p Pointer to the data structure containing the message handlers.
 * @param index The index of the message handler to remove.
 */
void remove_message_handler_by_index(void *p, int index);

/**
 * @brief Removes a message handler by its message.
 * 
 * @param p Pointer to the data structure containing the message handlers.
 * @param msg Pointer to the message associated with the handler to remove.
 */
void remove_message_handling(void *p, char *msg);

/**
 * @brief Adds a message handler.
 * 
 * @param peer Pointer to the peer structure.
 * @param msg Pointer to the message associated with the handler.
 * @param callback Pointer to the callback function to handle the message.
 * @param lifespan The lifespan of the message handler.
 * @param argc The number of arguments for the callback function.
 * @param data Pointer to the array of arguments for the callback function.
 */
void add_message_handler(void *peer, char *msg, int (*callback)(int, char *, void *, void **), int lifespan, int argc, void **data);

#pragma endregion