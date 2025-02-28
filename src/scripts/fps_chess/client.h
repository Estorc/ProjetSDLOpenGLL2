#pragma once

void *input_server(void *arg);
void launch_client(int socket);
void join_party(int id);
void create_party(const char *name);