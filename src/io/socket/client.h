#pragma once


#pragma region LOW LEVEL
int get_address(const char * name, int port, struct sockaddr_in *server_addr);
int connect_socket_server(const char * name, int port);
#pragma region HIGH LEVEL