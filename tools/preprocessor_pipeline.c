#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include "../src/io/stringio.c"

typedef enum {
    UNDERTERMINED,
    INSTANCE_METHOD,
    PROTOTYPE_METHOD,
    COMPLEX_METHOD
} CallFormat;

bool in_string(char * token) {
    char * rightquote = NULL;
    char * leftquote = NULL;
    char * cursor = token;
    while (*cursor != ';' && *cursor) cursor++;
    while (cursor != token) {
        cursor--;
        if (*cursor == '"') {
            if (!rightquote) rightquote = cursor;
            else leftquote = cursor;

            if (rightquote && leftquote) {
                leftquote = NULL;
                rightquote = NULL;
            }
        }
    }
    if (rightquote) return true;
    return false;
}

bool in_comment(char * token) {
    char * cursor = token;
    while (!(cursor[-1] == '*' && *cursor == '/') && *cursor) cursor++;
    if (*cursor) {
        int in_comment = true;
        while (cursor != token) {
            cursor--;
            if (cursor[-1] == '/' && *cursor == '*') {
                in_comment = false;
            }
        }
        if (in_comment) {
            return true;
        }
    }
    while (*cursor != '\n' && *cursor) {
        cursor--;
        if (cursor[-1] == '/' && *cursor == '/') {
            return true;
        }
    }
    return false;
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	FILE * source_file = fopen(argv[1], "r");
    if (!source_file) {
        fprintf(stderr, "The file \"%s\" does not exist!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    char * dest_filename = malloc(strlen(argv[1]) + 2);
    strcpy(dest_filename, argv[1]);
    strcat(dest_filename, "~");
    FILE * dest_file = fopen(dest_filename, "w");
    free(dest_filename);


    char * file_content = read_filef(source_file);

    CallFormat call_format = UNDERTERMINED;
    char * token = file_content;

    while ((token = strstr(token, "::"))) {

        if (in_string(token) || in_comment(token)) {
            token += 2;
            continue;
        }

        char * instance = NULL;
        char * method = NULL;
        char * arguments = NULL;
        char * cursor = NULL;
        char * start = NULL;
        char * end = NULL;

        if (token[-1] == ')') call_format = COMPLEX_METHOD;
        else call_format = INSTANCE_METHOD;

        cursor = token - 2;
        if (call_format == COMPLEX_METHOD) {
            for (int p_count = 1; p_count; cursor--) {
                if (*cursor == ')') p_count++;
                if (*cursor == '(') p_count--;
            }
            cursor++;
        } else {
            while (!isspace(*cursor)) cursor--;
            cursor++;
        }
        start = cursor;
        instance = malloc(token - cursor + 1);
        strncpy(instance, cursor, token - cursor);
        instance[token - cursor] = '\0';

        token += 2;
        cursor = token;
        while (*cursor != '(') cursor++;

        method = malloc(cursor - token + 1);
        strncpy(method, token, cursor - token);
        method[cursor - token] = '\0';

        char * arg_start = ++cursor;
        for (int p_count = 1; p_count; cursor++) {
            if (*cursor == ')') p_count--;
            if (*cursor == '(') p_count++;
        }
        cursor--;

        arguments = malloc(cursor - arg_start + 1);
        strncpy(arguments, arg_start, cursor - arg_start);
        arguments[cursor - arg_start] = '\0';

        end = cursor;

        printf("Got token: %s::%s(%s)\n", instance, method, arguments);

    }

    
    //fprintf(dest_file, "%s", file_content);
    fclose(source_file);
    fclose(dest_file);
    free(file_content);

	return 0;
}