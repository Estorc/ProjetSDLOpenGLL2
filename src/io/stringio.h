#include <string.h>

#ifndef STRINGIO_H
#define STRINGIO_H

char * read_file(const char * path);
char * get_folder_path(char * fullpath);
int find_string_index(char *str, const char **str_list, int list_size);

#endif