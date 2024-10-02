#include <string.h>

#ifndef STRINGIO_H
#define STRINGIO_H

const extern char* RELATIVE_PATH;

char * read_file(const char * path);
char * get_folder_path(char * fullpath);
char * relative_path(char * path);

#endif