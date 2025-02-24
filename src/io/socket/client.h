#pragma once

#pragma region LOW LEVEL
struct sockaddr_in get_address(const char * name, int port);
int connect_socket_server(const char * name, int port);
#pragma region HIGH LEVEL