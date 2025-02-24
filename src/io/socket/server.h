#pragma once

#pragma region LOW LEVEL
struct sockaddr_in create_address(int port);
int create_socket_server(int port);
int listen_socket(int sock);
#pragma region HIGH LEVEL