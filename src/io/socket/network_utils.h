#pragma once

int initiate_socket();
void close_socket(int sock);
struct socket_request_listener create_socket_request_listener(int sock);
int socket_request_listen(struct socket_request_listener * listener, int server_sock, int timeout);
int socket_request_receive(struct socket_request_listener * listener, int client_sock, char * buffer, int size, int timeout);