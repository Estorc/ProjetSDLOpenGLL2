#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

time_t getFileCreationTime(char *path) {
    struct stat attr;
    stat(path, &attr);
    if (stat(path, &attr) == 0) { // Check if stat is successful
        return attr.st_mtime;
    } else {
        return 0;
    }
}


#define CLASS_PATH "src/classes/"
#define SRC_PATH "src/"
#define CALLER_NAME "call_method"
#define CLASS_IMPORT_HEADER_FILENAME "import_class.h"
#define PROCESSED_PREFIX "__processed__/"
#define PROCESSED_HEADER_PREFIX "__PROCESSED__"
#define CONTAINER_TYPE_PREFIX "__containerType__"
#define NODE_TYPE_MARKER "_M_"
#define INSTRUCTION_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
#define PROCESSED_METHOD_PREFIX "__class_method_"
#define CLASS_TYPE_PREFIX "CLASS_TYPE_"
#define CLASS_PREFIX "class "
#define RETURN_VOID_POINTER "__retValueVP__"
#define RETURN_POINTER "__retValueP__"
#define PATH_SIZE 1000
#define LINE_SIZE 200
#define NAME_SIZE 100
#define TYPE_SIZE 100
#define ARGUMENTS_SIZE 100
#define METHODS_SIZE 100
#define CLASS_SIZE 100
#define TYPE_SIZE 100




#define BUFFER_SIZE 4096  // Read in 4KB chunks for efficiency

int compareFiles(FILE *fp1, FILE *fp2) {
    if (!fp1 || !fp2) {
        perror("Invalid file pointers");
        return -1; // Error: NULL file pointers
    }

    // Reopen files in binary mode
    fp1 = freopen(NULL, "rb", fp1);
    fp2 = freopen(NULL, "rb", fp2);

    if (!fp1 || !fp2) {
        perror("Failed to reopen files in binary mode");
        return -1;
    }

    unsigned char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    size_t bytesRead1, bytesRead2;

    do {
        bytesRead1 = fread(buffer1, 1, BUFFER_SIZE, fp1);
        bytesRead2 = fread(buffer2, 1, BUFFER_SIZE, fp2);

        if (bytesRead1 != bytesRead2 || memcmp(buffer1, buffer2, bytesRead1) != 0) {
            printf("Files are different\n");
            return 1; // Files are different
        }
    } while (bytesRead1 > 0);

    printf("Files are identical\n");
    return 0; // Files are identical
}


int copyFile(FILE *src, FILE *dest) {
    if (!src || !dest) {
        perror("Invalid file pointers");
        return -1;
    }

    src = freopen(NULL, "rb", src);
    dest = freopen(NULL, "wb", dest);

    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    return 0;  // Success
}




typedef struct Argument {
	char name[NAME_SIZE];
	char type[TYPE_SIZE];
} Argument;

typedef struct Method {
	char name[NAME_SIZE];
	char type[TYPE_SIZE];
	Argument arguments[ARGUMENTS_SIZE];
	int arguments_count;
} Method;

typedef struct Class {
	char name[NAME_SIZE];
	char type[TYPE_SIZE];
	Method methods[METHODS_SIZE];
	int methods_count;
} Class;

typedef struct ImportStruct {
	char corresponding_method[METHODS_SIZE][CLASS_SIZE][NAME_SIZE];
	char unique_method_name[METHODS_SIZE][NAME_SIZE];
	char unique_method_type[METHODS_SIZE][NAME_SIZE];
	char class_name[CLASS_SIZE][NAME_SIZE];
	char extends[CLASS_SIZE][NAME_SIZE];
	char type[TYPE_SIZE][NAME_SIZE];
	char (*filepath)[PATH_SIZE];
	char (*srcfilepath)[PATH_SIZE];
	int correponding_method_type_index[METHODS_SIZE][CLASS_SIZE];
	int type_count;
	int class_count;
	int unique_method_count;
} ImportStruct;


void clear_spaces(char *str) {
	char *i = str;
	char *j = str;
	while (*j != 0) {
		*i = *j++;
		if (*i != ' ') {
			i++;
		}
	}
	*i = 0;
}


int find_string_index(char *str, char (*str_list)[100], int list_size) {
	for (int i = 0; i < list_size; i++) {
		if (strcmp(str, str_list[i]) == 0) {
			return i;
		}
	}
	return -1; // Return -1 if the string is not found
}

void replace_char(char *str, char find, char replace) {
	for (; *str; str++) if (*str == find) *str = replace;
}

char * lowercase_string(char * str) {
	for (char *p = str; *p; ++p) *p = tolower(*p);
	return str;
}

char * uppercase_string(char * str) {
	for (char *p = str; *p; ++p) *p = toupper(*p);
	return str;
}

int update_cwd() {
    char exe_path[1024]; // Buffer to store the executable path.

    // Read the symbolic link /proc/self/exe to get the full path of the executable.
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("readlink failed");
        return -1;
    }

    exe_path[len] = '\0'; // Null-terminate the path.

    // Extract the directory from the full path.
    char *last_slash = strrchr(exe_path, '/');
    if (last_slash != NULL) {
        *last_slash = '\0'; // Remove the executable name to get the directory.
    } else {
        fprintf(stderr, "Error: Unable to determine directory of executable\n");
        return -1;
    }

	strcat(exe_path, "/../"); // Go up one directory.

    // Change the current working directory to the directory of the executable.
    if (chdir(exe_path) != 0) {
        perror("chdir failed");
        return -1;
    }

    // Print the current working directory to verify the change.
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != 0) {
        #ifdef DEBUG
        printf("Current working directory: %s\n", cwd);
        #endif
    } else {
        perror("getcwd failed");
    }
    return 0;
}

void create_caller_function(char * type, int type_index, FILE * src_file, FILE * header_file) {
	fprintf(header_file, "%s %s_%d(void (*func)(void *, va_list), ...);\n", type, CALLER_NAME, type_index);

	fprintf(src_file, "%s %s_%d(void (*func)(void *, va_list), ...) {\n", type, CALLER_NAME, type_index);
	fprintf(src_file, "    va_list args;\n");
	fprintf(src_file, "    va_start(args, func);\n");
	fprintf(src_file, "    %s value;\n", type);
	fprintf(src_file, "    func(&value, args);\n");
	fprintf(src_file, "    va_end(args);\n");
	fprintf(src_file, "    return value;\n");
	fprintf(src_file, "}\n");
}

int get_method_args(char *args, Argument *arg) {
	char *arg_start = args;
	char *arg_end = args;
	int arg_count = 0;
	while (*arg_end) {
		arg_end++;
		if (*arg_end == ',' || *arg_end == '\0') {
			while (*arg_start == ' ') arg_start++;
			char argstr[100];
			strncpy(argstr, arg_start, arg_end - arg_start);
			argstr[arg_end - arg_start] = '\0';

			if (strstr(argstr, "...")) {
				return arg_count;
			}
			
			char * argstr_end = argstr + strlen(argstr);
			while (*argstr_end == ' ') argstr_end--;
			while (*argstr_end != ' ' && *argstr_end != '*') argstr_end--;

			strcpy(arg[arg_count].name, argstr_end + 1);
			strncpy(arg[arg_count].type, argstr, argstr_end - argstr + 1);
			arg[arg_count].type[argstr_end - argstr + 1] = '\0';
			
			arg_count++;
			arg_start = arg_end + 1;
		}
	}
	return arg_count;
}


int get_class_method(char *line, Method *method) {
	int space_count = 0;
	while (*line == ' ' || *line == '\t') {
		line++;
		space_count++;
	}
	if (strspn(line, INSTRUCTION_CHARSET) == 0) return -1;
	char *expr_pos = strstr(line, "(");
	if (expr_pos) {
		char *func_end = strstr(line, ")");
		char *last_parenthesis = expr_pos;
		while ((last_parenthesis = strstr(last_parenthesis + 1, "(")) < func_end && last_parenthesis != NULL) {
			func_end = strstr(func_end + 1, ")");
		}
		if (func_end && func_end > expr_pos) {
			size_t expr_len = expr_pos - line;
			size_t func_args_len = func_end - (expr_pos+1);
			char func_args[100];
			strncpy(func_args, expr_pos+1, func_args_len);
			func_args[func_args_len] = '\0';


			char methodstr[100];
			strncpy(methodstr, line, expr_len);
			methodstr[expr_len] = '\0';
			
			char * methodstr_end = methodstr + strlen(methodstr);
			while (*methodstr_end == ' ') methodstr_end--;
			while (*methodstr_end != ' ' && *methodstr_end != '*') methodstr_end--;

			strcpy(method->name, methodstr_end + 1);
			strncpy(method->type, methodstr, methodstr_end - methodstr + 1);
			method->name[expr_len] = '\0';
			method->type[methodstr_end - methodstr] = '\0';

			if (strstr(method->type, "static")) {
				return -2;
			}

			if (func_args_len) method->arguments_count = get_method_args(func_args, &method->arguments[1]) + 1;
			else method->arguments_count = 1;

			return space_count;
		}
	}

	return -1;
}

bool get_class_name(char *line, Class *class) {
	char *expr_pos = strstr(line, CLASS_PREFIX);
	if (expr_pos) {
		size_t expr_len = strlen(line);
		expr_pos += strlen(CLASS_PREFIX);
		expr_pos += strspn(expr_pos, " ");
		expr_len = strspn(expr_pos, INSTRUCTION_CHARSET);
		strncpy(class->name, expr_pos, expr_len);
		class->name[expr_len] = '\0';
		return true;
	}
	return false;
}

void recursive_scan(char *path, char *base_path, struct dirent ***namelist, int (*filter)(const struct dirent *entry), int *n) {
	if (base_path == NULL) base_path = path;
	struct dirent **local_namelist;
	int local_n = scandir(path, &local_namelist, filter, alphasort);
	if (local_n < 0) {
		perror("scandir");
		return;
	}

	for (int i = 0; i < local_n; i++) {
		if (local_namelist[i]->d_type == DT_DIR) {
			char new_path[1024];
			snprintf(new_path, sizeof(new_path), "%s/%s", path, local_namelist[i]->d_name);
			recursive_scan(new_path, base_path, namelist, filter, n);
		} else {
			char *new_path = path + strlen(base_path);
			*namelist = realloc(*namelist, (*n + 1) * sizeof(struct dirent *));
			(*namelist)[*n] = malloc(sizeof(struct dirent));
			memcpy((*namelist)[*n], local_namelist[i], sizeof(struct dirent));
			if (strlen(new_path))
				snprintf((*namelist)[*n]->d_name, sizeof((*namelist)[*n]->d_name), "%s/%s", new_path + 1, local_namelist[i]->d_name);
			else
				snprintf((*namelist)[*n]->d_name, sizeof((*namelist)[*n]->d_name), "%s", local_namelist[i]->d_name);
			(*n)++;
		}
	}
	free(local_namelist);
}



int main(int argc, char ** argv) {

	update_cwd();
	struct dirent **namelist;
	int n = 0;
	int filter(const struct dirent *entry) {
		return (strstr(entry->d_name, ".class.c") != NULL && !strstr(entry->d_name, PROCESSED_PREFIX)) || (entry->d_type == DT_DIR && entry->d_name[0] != '.' && entry->d_name[0] != '_');
	}

	char dirpath[PATH_SIZE];
	strcpy(dirpath, SRC_PATH);
	strcat(dirpath, PROCESSED_PREFIX);
	mkdir(dirpath, 0755);
	recursive_scan(SRC_PATH, NULL, &namelist, filter, &n);


	if (n < 0) {
		perror("scandir");
		return 1;
	} else {
		static ImportStruct import_struct = {0};
		import_struct.filepath = malloc(n * sizeof(char[PATH_SIZE]));
		import_struct.srcfilepath = malloc(n * sizeof(char[PATH_SIZE]));
		// declare void because it is the default return type
		import_struct.type_count = 1;
		strcpy(import_struct.type[0], "void");
		FILE * source_file;
		FILE * processed_file;
		FILE * temp_processed_header_file;
		FILE * processed_header_file;
		bool need_rewriting_imports = false;
		char lwname[NAME_SIZE];
		char uppername[NAME_SIZE];
		for (int i = 0; i < n; i++) {
			char *filepath = malloc(strlen(namelist[i]->d_name) + strlen(SRC_PATH) + 1);
			filepath[0] = 0;
			strcat(filepath, SRC_PATH);
			strcat(filepath, namelist[i]->d_name);
			source_file = fopen(filepath, "r");
			if (!source_file) {
				fprintf(stderr, "Le fichier \"%s\" n'existe pas!", filepath);
				return -1;
			}

			time_t lastEditTime = getFileCreationTime(filepath);
			strcpy(import_struct.srcfilepath[i], filepath);

			filepath = realloc(filepath, strlen(namelist[i]->d_name) + strlen(SRC_PATH) + strlen(PROCESSED_PREFIX) + 1);
			filepath[0] = 0;
			strcat(filepath, SRC_PATH);
			strcat(filepath, PROCESSED_PREFIX);
			replace_char(namelist[i]->d_name, '/', '_');
			strcat(filepath, namelist[i]->d_name);

			time_t lastProcessedTime = 0;
			lastProcessedTime = getFileCreationTime(filepath);

			if (lastEditTime < lastProcessedTime) {
				processed_file = fopen("/dev/null", "w");
				processed_header_file = NULL;
				if (processed_file == NULL) {
					perror("Error opening /dev/null");
					return 1;
				}
				filepath[strlen(filepath) - 1] = 'h';
			} else {
				processed_file = fopen(filepath, "w");
				printf("Processing file -> %s\n", filepath);
				filepath[strlen(filepath) - 1] = 'h';
				processed_header_file = fopen(filepath, "r");
				if (!processed_header_file) {
					processed_header_file = fopen(filepath, "w");	
				}
			}

			temp_processed_header_file = tmpfile();

			strcpy(import_struct.filepath[i], filepath + strlen(SRC_PATH));

			size_t filename_len = strspn(namelist[i]->d_name, INSTRUCTION_CHARSET);
			char filename[100];
			strncpy(filename, namelist[i]->d_name, filename_len);
			filename[filename_len] = '\0';
			uppercase_string(filename);
			
			fprintf(temp_processed_header_file, "#ifndef %s%s_H\n", PROCESSED_HEADER_PREFIX, filename);
			fprintf(temp_processed_header_file, "#define %s%s_H\n", PROCESSED_HEADER_PREFIX, filename);

			free(filepath);
			
			char line[LINE_SIZE];
			bool in_class = false;
			bool in_method = false;
			bool in_static_method = false;
			bool in_return = false;
			int space_count = -1;
			int brace_count = 0;
			int line_count = 0;
			Class current_class;

			fprintf(processed_file, "#include <stdarg.h>\n");
			fprintf(processed_file, "#include \"../raptiquax.h\"\n");
			fprintf(processed_file, "#include \"../classes/classes.h\"\n");

			do {
				line_count++;
				int j = 0;
				for (; line[j-1] != '\n' && !feof(source_file); j++) line[j] = fgetc(source_file);
				line[j] = 0;
				if (feof(source_file)) line[j-1] = 0;
				if (in_class) {
					Method *current_method = &current_class.methods[current_class.methods_count];

					if (strstr(line, "{")) brace_count++;
					if (strstr(line, "}") != NULL) {
						brace_count--;
						in_class = in_method || in_static_method || (brace_count > -1);
						if (!brace_count && in_static_method) {
							in_static_method = false;
							fprintf(processed_file, "}\n");
							continue;
						}
						if (!brace_count && in_method) {
							current_class.methods_count++;
							in_method = false;
							fprintf(processed_file, "}\n");
							continue;
						}
						if (!in_class) {
							brace_count = 0;

							for (size_t i = 0; i < current_class.methods_count; i++) {
								fprintf(temp_processed_header_file, "void %s%s_%s(void * %s, va_list args);\n", PROCESSED_METHOD_PREFIX, lwname, current_class.methods[i].name, RETURN_VOID_POINTER);

								int index;
								if ((index = find_string_index(current_class.methods[i].name, import_struct.unique_method_name, import_struct.unique_method_count)) == -1) {
									strcpy(import_struct.unique_method_name[import_struct.unique_method_count], current_class.methods[i].name);
									strcpy(import_struct.unique_method_type[import_struct.unique_method_count], current_class.methods[i].type);
									index = import_struct.unique_method_count;
									import_struct.unique_method_count++;
									for (int i = 0; i < CLASS_SIZE; i++) {
										strcpy(import_struct.corresponding_method[index][i], "NULL");
									}
								}
								sprintf(import_struct.corresponding_method[index][import_struct.class_count-1], "%s%s_%s", PROCESSED_METHOD_PREFIX, lwname, current_class.methods[i].name);
							}

							continue;

						}
					}


					if (in_method) {
						char * text = line;
						//while (space_count > text - line && (*text == ' ' || *text == '\t')) text++;
						char * return_pos;
						if ((return_pos = strstr(line, "return "))) {
							text[return_pos - text] = 0;
							fprintf(processed_file, "%s", text);
							text = return_pos;
							text += strlen("return ");
							fprintf(processed_file, "*%s = ", RETURN_POINTER);
							in_return = true;
						}
						if (in_return) {
							while (*text) {
								if (*text == ';') {
									fprintf(processed_file, ";return;\n");
									in_return = false;
									break;
								} else fputc(*text, processed_file);
								text++;
							}
						} else fprintf(processed_file, "%s", text);
					} else {
						if (strstr(line, "private:")) {
							continue;
						}
						if (strstr(line, "public:")) {
							continue;
						}
						if (strstr(line, CONTAINER_TYPE_PREFIX)) {
							char *expr_pos = strstr(line, CONTAINER_TYPE_PREFIX);
							expr_pos += strlen(CONTAINER_TYPE_PREFIX);
							expr_pos += strspn(expr_pos, " ");
							size_t expr_len = strcspn(expr_pos, "\n");
							strncpy(current_class.type, expr_pos, expr_len);
							current_class.type[expr_len] = '\0';
							continue;
						}
						if (!in_static_method && (space_count = get_class_method(line, current_method)) != -1) {
							if (space_count == -2) {
								fprintf(processed_file, "%s", line);
								in_static_method = true;
							} else {
								clear_spaces(current_method->type);
								int type_index = find_string_index(current_method->type, import_struct.type, import_struct.type_count);
								if (type_index == -1) {
									strcpy(import_struct.type[import_struct.type_count], current_method->type);
									type_index = import_struct.type_count;
									create_caller_function(current_method->type, type_index, processed_file, temp_processed_header_file);
									import_struct.type_count++;
								}
								import_struct.correponding_method_type_index[current_class.methods_count][import_struct.class_count-1] = type_index;
								fprintf(processed_file, "#line %d \"%s\"\n", line_count, import_struct.srcfilepath[i]);
								strcpy(current_method->arguments[0].name, "this");
								strcpy(current_method->arguments[0].type, current_class.type);

								fprintf(processed_file, "void %s%s_%s(void * %s, va_list args) {", PROCESSED_METHOD_PREFIX, lwname, current_method->name, RETURN_VOID_POINTER);
								if (strcmp(current_method->type, "void") != 0) {
									fprintf(processed_file, "%s * %s = (%s *) %s;", current_method->type, RETURN_POINTER, current_method->type, RETURN_VOID_POINTER);
								}
								for (size_t j = 0; j < current_method->arguments_count; j++) {
									char * arg_type_end = current_method->arguments[j].name + strspn(current_method->arguments[j].name, INSTRUCTION_CHARSET);
									fprintf(processed_file, "%s %s = va_arg(args, %s%s);", current_method->arguments[j].type, current_method->arguments[j].name, current_method->arguments[j].type, arg_type_end);
								}
								fprintf(processed_file, "(void)this;\n");

								in_method = true;
							}
						} else {
							fprintf(processed_file, "%s", line);
						}
					}
				} else {
					if (strstr(line, "#")) {
						if (strstr(line, "#include \"")) {
							fprintf(processed_file, "#include \"../%s", line + strlen("#include \""));	// ../ because we are in the __processed__ directory
							fprintf(temp_processed_header_file, "#include \"../src/%s", line + strlen("#include \"")); // ../src because we are in the __processed__ directory
						} else {
							fprintf(processed_file, "%s", line);
						}
					}
					in_class = get_class_name(line, &current_class);
					if (in_class) {

						strcpy(uppername, current_class.name);
						fprintf(processed_file, "static unsigned __type__ __attribute__((unused)) = %s%s;", CLASS_TYPE_PREFIX, uppercase_string(uppername));

						strcpy(lwname, current_class.name);
						lowercase_string(lwname);

						char *extends = strstr(line, ": public");
						if (extends) {
							extends += strlen(": public");
							extends += strspn(extends, " ");
							size_t extends_len = strspn(extends, INSTRUCTION_CHARSET);
							strncpy(import_struct.extends[import_struct.class_count], extends, extends_len);
							import_struct.extends[import_struct.class_count][extends_len] = '\0';
						} else {
							import_struct.extends[import_struct.class_count][0] = 0;
						}

						current_class.methods_count = 0;
						
						strcpy(import_struct.class_name[import_struct.class_count], current_class.name);
						import_struct.class_count++;
					}
				}


			} while (!feof(source_file));
			fprintf(temp_processed_header_file, "#endif\n");

			rewind(temp_processed_header_file);
			if (processed_header_file != NULL && compareFiles(temp_processed_header_file, processed_header_file)) {
				printf("Changes detected in header file\n");
				rewind(temp_processed_header_file);
				copyFile(temp_processed_header_file, processed_header_file);
				need_rewriting_imports = true;
			}

			fclose(source_file);
			fclose(processed_file);
			fclose(temp_processed_header_file);
			if (processed_header_file != NULL) fclose(processed_header_file);
		}
		if (need_rewriting_imports) {

			char *filepath = malloc(strlen(CLASS_PATH) + strlen(CLASS_IMPORT_HEADER_FILENAME));
			filepath[0] = 0;
			strcat(filepath, CLASS_PATH);
			strcat(filepath, CLASS_IMPORT_HEADER_FILENAME);
			FILE * json_data_file = fopen("./tools/import_class.json", "w");
			FILE * import_class_header_file = fopen(filepath, "w");
			free(filepath);

			fprintf(import_class_header_file, "// This file is generated by the class_tools.c tool\n");
			for (int i = 0; i < n; i++) {
				fprintf(import_class_header_file, "#include \"../%s\"\n", import_struct.filepath[i]);
			}
			fprintf(import_class_header_file, "#ifndef IMPORT_CLASS_H\n");
			fprintf(import_class_header_file, "#define IMPORT_CLASS_H\n");
			fprintf(import_class_header_file, "typedef enum ClassType {\n");
			fprintf(json_data_file, "{\n");
			fprintf(json_data_file, "\t\"type_associations\": {\n");
			for (size_t i = 0; i < import_struct.class_count; i++) {
				strcpy(uppername, import_struct.class_name[i]);
				fprintf(json_data_file, "\t\t\"%s\": \"%s%s\"%c\n", import_struct.class_name[i], CLASS_TYPE_PREFIX, uppercase_string(uppername), i < import_struct.class_count - 1 ? ',' : ' ');
				fprintf(import_class_header_file, "\t%s%s,\n", CLASS_TYPE_PREFIX, uppercase_string(uppername));
			}
			fprintf(json_data_file, "\t},\n");
			fprintf(import_class_header_file, "\t%sCOUNT\n", CLASS_TYPE_PREFIX);
			fprintf(import_class_header_file, "} ClassType;\n");
			fprintf(import_class_header_file, "struct MethodsCorrespondance {\n");
			for (size_t i = 0; i < import_struct.unique_method_count; i++) {
				fprintf(import_class_header_file, "\tvoid (*%s[%d])(void * %s, va_list args);\n", import_struct.unique_method_name[i], import_struct.class_count, RETURN_VOID_POINTER);
			}
			fprintf(import_class_header_file, "};\n");

			fprintf(import_class_header_file, "struct ClassManager {\n");
			fprintf(import_class_header_file, "\tstruct MethodsCorrespondance methodsCorrespondance;\n");
			fprintf(import_class_header_file, "\tClassType extends[%d];\n", import_struct.class_count);
			fprintf(import_class_header_file, "\tconst char * class_names[%d];\n", import_struct.class_count);
			fprintf(import_class_header_file, "};\n");

			fprintf(import_class_header_file, "extern const struct ClassManager classManager;\n");
			fprintf(import_class_header_file, "#define BUILD_CLASS_METHODS_CORRESPONDANCE(classManager) const struct ClassManager classManager = {\\\n");
			fprintf(import_class_header_file, "\t.methodsCorrespondance = {\\\n");
			fprintf(json_data_file, "\t\"method_index\": {\n");
			for (size_t i = 0; i < import_struct.unique_method_count; i++){
				fprintf(json_data_file, "\t\t\"%s\": %ld%c\n", import_struct.unique_method_name[i], i, i < import_struct.unique_method_count - 1 ? ',' : ' ');
				fprintf(import_class_header_file, "\t\t.%s = {", import_struct.unique_method_name[i]);
				for (size_t j = 0; j < import_struct.class_count; j++) {
					size_t method_index = j;
					size_t extends_index = find_string_index(import_struct.extends[method_index], import_struct.class_name, import_struct.class_count);
					while (strcmp(import_struct.corresponding_method[i][method_index], "NULL") == 0 && extends_index != -1) {
						method_index = extends_index;
						extends_index = find_string_index(import_struct.extends[method_index], import_struct.class_name, import_struct.class_count);
					}
					fprintf(import_class_header_file, "%s", import_struct.corresponding_method[i][method_index]);
					if (j < import_struct.class_count - 1) {
						fprintf(import_class_header_file, ", ");
					}
				}
				fprintf(import_class_header_file, "},\\\n");
			}
			fprintf(json_data_file, "\t},\n");
			fprintf(import_class_header_file, "\t},\\\n");

			fprintf(json_data_file, "\t\"type_caller\": {\n");
			for (size_t i = 0; i < import_struct.class_count; i++){
				fprintf(json_data_file, "\t\t\"%s\": [", import_struct.class_name[i]);
				for (size_t j = 0; j < import_struct.unique_method_count; j++) {
					fprintf(json_data_file, "\"%s_%d\"%c", CALLER_NAME, import_struct.correponding_method_type_index[j][i], j < import_struct.unique_method_count - 1 ? ',' : ' ');
				}
				fprintf(json_data_file, "]%c\n", i < import_struct.class_count - 1 ? ',' : ' ');
			}
			fprintf(json_data_file, "\t},\n");

			fprintf(json_data_file, "\t\"type_cast\": {\n");
			for (size_t i = 0; i < import_struct.type_count; i++){
				fprintf(json_data_file, "\t\t\"%s\": \"%s_%ld\"%c\n", import_struct.type[i], CALLER_NAME, i, i < import_struct.type_count - 1 ? ',' : ' ');
			}
			fprintf(json_data_file, "\t}\n");


			fprintf(import_class_header_file, "\t.extends = {");
			for (size_t i = 0; i < import_struct.class_count; i++) {
				fprintf(import_class_header_file, "%d", find_string_index(import_struct.extends[i], import_struct.class_name, import_struct.class_count));
				if (i < import_struct.class_count - 1) fprintf(import_class_header_file, ", ");
			}
			fprintf(import_class_header_file, "},\\\n");
			fprintf(import_class_header_file, "\t.class_names = {");
			for (size_t i = 0; i < import_struct.class_count; i++) {
				fprintf(import_class_header_file, "\"%s\"", import_struct.class_name[i]);
				if (i < import_struct.class_count - 1) fprintf(import_class_header_file, ", ");
			}
			fprintf(import_class_header_file, "}\\\n");
			fprintf(import_class_header_file, "};\n");
			
			fprintf(import_class_header_file, "#endif\n");
			fprintf(json_data_file, "}\n");
			
			fclose(json_data_file);
			fclose(import_class_header_file);
		}
		free(namelist);
	}
}