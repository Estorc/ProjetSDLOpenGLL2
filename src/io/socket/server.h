#pragma once

struct sockaddr_in create_address(int port);
int create_socket_server(int port);
int listen_socket(int sock);