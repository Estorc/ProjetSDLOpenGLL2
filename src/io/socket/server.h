#pragma once
/**
 * @file server.h
 * @brief Header file for socket server functions.
 *
 * This file contains the declarations of functions used to create and manage
 * a socket server.
 */


#pragma region LOW LEVEL

/**
 * @brief Creates a sockaddr_in structure for the given port.
 *
 * This function initializes a sockaddr_in structure with the specified port
 * and sets the address to INADDR_ANY.
 *
 * @param port The port number to bind the address to.
 * @return A sockaddr_in structure initialized with the given port.
 */
struct sockaddr_in create_address(int port);

/**
 * @brief Creates a socket server and binds it to the specified port.
 *
 * This function creates a socket, binds it to the given port, and sets it up
 * to listen for incoming connections.
 *
 * @param port The port number to bind the socket to.
 * @return The file descriptor of the created socket, or -1 on error.
 */
int create_socket_server(int port);

/**
 * @brief Listens for incoming connections on the specified socket.
 *
 * This function sets the socket to listen for incoming connections.
 *
 * @param sock The file descriptor of the socket to listen on.
 * @return 0 on success, or -1 on error.
 */
int listen_socket(int sock);
#pragma region HIGH LEVEL