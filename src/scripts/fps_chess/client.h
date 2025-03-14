#pragma once

void *input_server(void *arg);
void launch_client(int socket);
void split_user_from_content(char *msg, char **user, char **content);
void join_party(int id);
void create_party(const char *name);