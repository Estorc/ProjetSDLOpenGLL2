#include <string.h>

#ifndef STRINGIO_H
#define STRINGIO_H

char * read_file(const char * path);
char * get_folder_path(char * fullpath);
int find_string_index(char *str, const char **str_list, int list_size);
int is_utf8_start_byte(unsigned char c);
void remove_last_utf8_char(char *inputBuffer);

#endif