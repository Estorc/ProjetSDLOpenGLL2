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

int find_string_index(char *str, const char **str_list, int list_size) {
	for (int i = 0; i < list_size; i++) {
		if (strcmp(str, str_list[i]) == 0) {
			return i;
		}
	}
	return -1; // Return -1 if the string is not found
}

// Function to check if a byte is the start of a UTF-8 character (i.e., 0xC0 or above).
int is_utf8_start_byte(unsigned char c) {
    return (c & 0xC0) != 0x80;  // Check if it's not a continuation byte (0x80-0xBF).
}

// Function to remove the last UTF-8 character in the input string.
void remove_last_utf8_char(char *inputBuffer) {
    size_t len = strlen(inputBuffer);
    
    if (len == 0) return;

    // Move backward to find the start of the last character
    size_t i = len - 1;
    while (i > 0 && !is_utf8_start_byte((unsigned char)inputBuffer[i])) {
        i--;
    }
    
    // Null-terminate the string at the start of the last character
    inputBuffer[i] = '\0';
}