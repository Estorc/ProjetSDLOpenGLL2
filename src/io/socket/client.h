#pragma once
/**
 * @file client.h
 * @brief Header file for socket client operations.
 */

#pragma region LOW LEVEL
/**
 * @brief Resolves the address of the server.
 *
 * This function resolves the address of the server given its name and port number.
 *
 * @param name The name of the server.
 * @param port The port number of the server.
 * @param server_addr A pointer to a sockaddr_in structure where the resolved address will be stored.
 * @return An integer indicating the success or failure of the address resolution.
 *         Returns 0 on success, or a negative value on failure.
 */
int get_address(const char * name, int port, struct sockaddr_in *server_addr);

/**
 * @brief Connects to a socket server.
 *
 * This function establishes a connection to a socket server given its name and port number.
 *
 * @param name The name of the server.
 * @param port The port number of the server.
 * @return An integer representing the socket file descriptor on success, or a negative value on failure.
 */
int connect_socket_server(const char * name, int port);
#pragma region HIGH LEVEL