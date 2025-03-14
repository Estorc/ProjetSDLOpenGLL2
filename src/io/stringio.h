#include <string.h>

#ifndef STRINGIO_H
#define STRINGIO_H

#ifdef _WIN32
#define strtok_s strtok_s
#define strtok_r strtok_s
#else
#define strtok_s strtok_r
#define strtok_r strtok_r
#endif

/**
 * @file stringio.h
 * @brief Provides utility functions for string and file operations.
 * 
 * This header file contains declarations for functions that handle
 * reading files, manipulating file paths, searching strings, and
 * processing UTF-8 encoded strings.
 */

/**
 * @defgroup StringIO String and File I/O Utilities
 * @brief Utility functions for string and file operations.
 * @{
 */

/**
 * @brief Reads the contents of a file into a string.
 * 
 * This function reads the contents of a file that is already open and
 * returns them as a dynamically allocated string. The caller is responsible
 * for freeing the allocated memory.
 * 
 * @param file The file to read from.
 * @return A pointer to the dynamically allocated string containing the file contents.
 * @note The caller is responsible for freeing the allocated memory.
 * @note The file must be open and readable.
 */
 char * read_filef(FILE * file);

/**
 * @brief Reads the contents of a file into a string.
 * 
 * This function opens the file specified by the given path, reads its
 * contents, and returns them as a dynamically allocated string. The caller
 * is responsible for freeing the allocated memory.
 * 
 * @param path The path to the file to be read.
 * @return A pointer to the dynamically allocated string containing the file contents.
 * @note The caller is responsible for freeing the allocated memory.
 */
char * read_file(const char * path);

/**
 * @brief Extracts the folder path from a full file path.
 * 
 * This function takes a full file path and returns the folder path portion
 * of it. The returned string is dynamically allocated, and the caller is
 * responsible for freeing the allocated memory.
 * 
 * @param fullpath The full file path from which to extract the folder path.
 * @return A pointer to the dynamically allocated string containing the folder path.
 * @note The caller is responsible for freeing the allocated memory.
 */
char * get_folder_path(const char * fullpath);

/**
 * @brief Converts backslashes in a path to forward slashes.
 * 
 * This function takes a path string and converts any backslashes to forward
 * slashes. The returned string is dynamically allocated, and the caller is
 * responsible for freeing the allocated memory.
 * 
 * @param path The path to convert.
 * @return A pointer to the dynamically allocated string containing the converted path.
 * @note The caller is responsible for freeing the allocated memory.
 */
char * convert_path(const char * path);

/**
 * @brief Concatenates two file paths.
 * 
 * This function concatenates two file paths, adding a path separator between
 * them if necessary. The returned string is dynamically allocated, and the
 * caller is responsible for freeing the allocated memory.
 * 
 * @param path1 The first part of the path.
 * @param path2 The second part of the path.
 * @return A pointer to the dynamically allocated string containing the concatenated path.
 * @note The caller is responsible for freeing the allocated memory.
 */
char * concat_path(const char * path1, const char * path2);

/**
 * @brief Finds the index of a string in a list of strings.
 * 
 * This function searches for a given string in a list of strings and returns
 * the index of the first occurrence. If the string is not found, the function
 * returns -1.
 * 
 * @param str The string to search for.
 * @param str_list The list of strings to search within.
 * @param list_size The number of strings in the list.
 * @return The index of the first occurrence of the string, or -1 if not found.
 */
int find_string_index(char *str, const char **str_list, int list_size);

/**
 * @brief Formats escaped newlines in a string.
 * 
 * This function replaces escaped newline sequences in a string with actual
 * newline characters. It modifies the input string in place.
 * 
 * @param str The string to format.
 * @return A pointer to the modified string.
 * @note This function modifies the input string in place.
 */
char * format_escaped_newlines(char *str);

/**
 * @brief Checks if a byte is a valid UTF-8 start byte.
 * 
 * This function determines whether a given byte is a valid start byte for
 * a UTF-8 encoded character. A valid start byte is any byte that can begin
 * a multi-byte UTF-8 character.
 * 
 * @param c The byte to check.
 * @return 1 if the byte is a valid UTF-8 start byte, 0 otherwise.
 */
int is_utf8_start_byte(unsigned char c);

/**
 * @brief Removes the last UTF-8 character from a string.
 * 
 * This function removes the last UTF-8 encoded character from the given
 * input buffer. It correctly handles multi-byte UTF-8 characters and adjusts
 * the string length accordingly.
 * 
 * @param inputBuffer The string from which to remove the last UTF-8 character.
 */
void remove_last_utf8_char(char *inputBuffer);

/** @} */ // end of StringIO group

#endif