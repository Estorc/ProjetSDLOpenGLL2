#include "../types.h"
#include "stringio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * Reads the entire contents of a text file into a dynamically allocated string.
 *
 * @param path {const char *} The path to the file to be read. This should be a 
 *                            null-terminated string representing the file's 
 *                            location in the filesystem.
 * @return {char *} A pointer to a dynamically allocated string containing the 
 *                  contents of the file. The string is null-terminated. If 
 *                  the file could not be opened or read, returns NULL.
 * 
 * This function opens a file in read mode and reads its contents character by 
 * character. It dynamically allocates memory for the string and expands the 
 * buffer as needed until the end of the file is reached. The final string 
 * is null-terminated, making it suitable for use with standard string functions.
 *
 * Important Notes:
 * - The caller is responsible for freeing the memory allocated for the string 
 *   returned by this function to avoid memory leaks.
 * - If the file does not exist or cannot be opened, the function will return 
 *   NULL.
 * - The function assumes the file is a text file and reads characters until 
 *   the end of the file is reached. 
 * - It is recommended to check the return value for NULL to handle errors 
 *   appropriately.
 */

char * read_file(const char * path) {
    FILE * file = fopen(path, "r");
    int bufferSize = 8, bufferPos = 0;
    char c;
    char *s = malloc(sizeof(char) * bufferSize);
    POINTER_CHECK(s);
    do {
        c = getc(file);
        s[bufferPos++] = c;
        if (bufferPos >= bufferSize) s = realloc(s, sizeof(char) * (bufferSize <<= 1));
        POINTER_CHECK(s);
    } while (c != -1);
    s = realloc(s, sizeof(char) * (bufferPos + 1));
    POINTER_CHECK(s);
    s[bufferPos-1] = 0;
    return s;
}

char * get_folder_path(char * fullpath) {
    char * path = malloc(sizeof(char) * (strlen(fullpath) + 1));
    strcpy(path, fullpath);
    int slash_count = 0;
    for (char * ptr = path; *ptr; ptr++) if (*ptr == '/' || *ptr == '\\') slash_count++;
    if (!slash_count) {
        path[0] = 0;
        return path;
    }
    for (char * ptr = path; slash_count; ptr++) if (*ptr == '/' || *ptr == '\\') if (!(--slash_count) && *(ptr+1)) *(ptr+1) = 0;
    return path;
}

char * relative_path(char * path) {
    char * rel_path = malloc((strlen(path) + strlen(RELATIVE_PATH) + 1) * sizeof(char));
    strcpy(rel_path, RELATIVE_PATH);
    strcat(rel_path, path);
    return rel_path;
}

int str_includes(char *src, char *occ) {
	int start = 0;
	for (char * str = src; *str; str++) {
		if (*str == *occ) occ++, start++;
		else if (start) return 0;
		if (!*occ) return 1;
	}
	return 0;
}