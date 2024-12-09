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


#define CLASS_PATH "../src/classes/"
#define CLASS_IMPORT_HEADER_FILENAME "import_class.h"
#define PROCESSED_PREFIX "__processed__/"
#define PROCESSED_HEADER_PREFIX "__PROCESSED__"
#define SYMBOL_PREFIX "__symbol__"
#define CONTAINER_TYPE_PREFIX "__containerType__"
#define NODE_TYPE_MARKER "_M_"
#define INSTRUCTION_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
#define PROCESSED_METHOD_PREFIX "__class_method_"
#define CLASS_TYPE_PREFIX "CLASS_TYPE_"
#define CLASS_PREFIX "class "
#define PATH_SIZE 1000
#define LINE_SIZE 200
#define NAME_SIZE 100
#define TYPE_SIZE 100
#define SYMBOL_SIZE 20
#define METHOD_CONTENT_SIZE 10000
#define ARGUMENTS_SIZE 100
#define METHODS_SIZE 100
#define CLASS_SIZE 100

typedef struct Argument {
	char name[NAME_SIZE];
	char type[TYPE_SIZE];
} Argument;

typedef struct Method {
	char name[NAME_SIZE];
	char type[TYPE_SIZE];
	Argument arguments[ARGUMENTS_SIZE];
	char content[METHOD_CONTENT_SIZE];
	int arguments_count;
} Method;

typedef struct Class {
	char name[NAME_SIZE];
	char symbol[SYMBOL_SIZE];
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
	char (*filepath)[PATH_SIZE];
	int class_count;
	int unique_method_count;
} ImportStruct;


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
	char *expr_pos = strstr(line, "(");
	if (expr_pos) {
		char *func_end = strstr(line, ")");
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
			method->type[methodstr_end - methodstr + 1] = '\0';

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

	recursive_scan(CLASS_PATH, NULL, &namelist, filter, &n);


	if (n < 0) {
		perror("scandir");
		return 1;
	} else {
		ImportStruct import_struct = {0};
		import_struct.filepath = malloc(n * sizeof(char[PATH_SIZE]));
		FILE * source_file;
		FILE * processed_file;
		FILE * processed_header_file;
		bool need_rewriting_imports = false;
		for (int i = 0; i < n; i++) {
			char *filepath = malloc(strlen(namelist[i]->d_name) + strlen(CLASS_PATH));
			filepath[0] = 0;
			strcat(filepath, CLASS_PATH);
			strcat(filepath, namelist[i]->d_name);
			source_file = fopen(filepath, "r");
			if (!source_file) {
				fprintf(stderr, "Le fichier \"%s\" n'existe pas!", filepath);
				return -1;
			}

			printf("line: %s\n", filepath);

			time_t lastEditTime = getFileCreationTime(filepath);
			printf("Processing file %s -> ", filepath);

			free(filepath);
			filepath = malloc(strlen(namelist[i]->d_name) + strlen(CLASS_PATH) + strlen(PROCESSED_PREFIX));
			filepath[0] = 0;
			strcat(filepath, CLASS_PATH);
			strcat(filepath, PROCESSED_PREFIX);
			replace_char(namelist[i]->d_name, '/', '_');
			strcat(filepath, namelist[i]->d_name);

			time_t lastProcessedTime = 0;
			lastProcessedTime = getFileCreationTime(filepath);

			if (lastEditTime < lastProcessedTime) {
				printf(" - Already processed\n");
				processed_file = fopen("/dev/null", "w");
				processed_header_file = fopen("/dev/null", "w");
				if (processed_file == NULL) {
					perror("Error opening /dev/null");
					return 1;
				}
				filepath[strlen(filepath) - 1] = 'h';
			} else {
				need_rewriting_imports = true;
				processed_file = fopen(filepath, "w");
				printf("%s, ", filepath);
				filepath[strlen(filepath) - 1] = 'h';
				processed_header_file = fopen(filepath, "w");
				printf("%s\n", filepath);
			}

			strcpy(import_struct.filepath[i], filepath + strlen(CLASS_PATH));
			printf("%s\n", import_struct.filepath[i]);

			size_t filename_len = strspn(namelist[i]->d_name, INSTRUCTION_CHARSET);
			char filename[100];
			strncpy(filename, namelist[i]->d_name, filename_len);
			uppercase_string(filename);
			
			fprintf(processed_header_file, "#ifndef %s%s_H\n", PROCESSED_HEADER_PREFIX, filename);
			fprintf(processed_header_file, "#define %s%s_H\n", PROCESSED_HEADER_PREFIX, filename);

			free(filepath);
			
			char line[LINE_SIZE];
			bool in_class = false;
			bool in_method = false;
			int space_count = -1;
			int brace_count = 0;
			Class current_class;

			fprintf(processed_file, "#include <stdarg.h>\n");
			fprintf(processed_file, "#include \"../../classes/classes.h\"\n");
			fprintf(processed_file, "#include \"../../types.h\"\n");

			do {
				int j = 0;
				for (; line[j-1] != '\n' && !feof(source_file); j++) line[j] = fgetc(source_file);
				line[j] = 0;
				if (in_class) {
					Method *current_method = &current_class.methods[current_class.methods_count];
					if (in_method) {
						char * text = line;
						while (space_count > text - line && (*text == ' ' || *text == '\t')) text++;
						strcat(current_method->content, text);
					} else {
						if ((space_count = get_class_method(line, current_method)) != -1) {
							strcpy(current_method->arguments[0].name, "this");
							strcpy(current_method->arguments[0].type, current_class.type);
							printf("\tMethod type: %s\n", current_method->type);
							printf("\tMethod name: %s\n", current_method->name);
							for (size_t i = 0; i < current_method->arguments_count; i++) {
								printf("\t\tArgument type: %s\n", current_method->arguments[i].type);
								printf("\t\tArgument name: %s\n", current_method->arguments[i].name);
							}
							current_method->content[0] = 0;
							in_method = true;
						}
						if (strstr(line, SYMBOL_PREFIX)) {
							char *expr_pos = strstr(line, SYMBOL_PREFIX);
							expr_pos += strlen(SYMBOL_PREFIX);
							expr_pos += strspn(expr_pos, " ");
							size_t expr_len = strspn(expr_pos, INSTRUCTION_CHARSET);
							strncpy(current_class.symbol, expr_pos, expr_len);
							current_class.symbol[expr_len] = '\0';
							printf("Class symbol: %s\n", current_class.symbol);
						}
						if (strstr(line, CONTAINER_TYPE_PREFIX)) {
							char *expr_pos = strstr(line, CONTAINER_TYPE_PREFIX);
							expr_pos += strlen(CONTAINER_TYPE_PREFIX);
							expr_pos += strspn(expr_pos, " ");
							size_t expr_len = strcspn(expr_pos, "\n");
							strncpy(current_class.type, expr_pos, expr_len);
							current_class.type[expr_len] = '\0';
							printf("Class type: %s\n", current_class.type);
						}
					}
					if (strstr(line, "{")) brace_count++;
					if (strstr(line, "}") != NULL) {
						brace_count--;
						in_class = in_method;
						if (!brace_count && in_method) {
							current_class.methods_count++;
							in_method = false;
						}
						if (!in_class) {
							brace_count = 0;
							char lwname[100];
							strcpy(lwname, current_class.name);
							lowercase_string(lwname);

							for (size_t i = 0; i < current_class.methods_count; i++) {
								fprintf(processed_file, "%s%s%s_%s(unsigned type, ...) {\n", current_class.methods[i].type, PROCESSED_METHOD_PREFIX, lwname, current_class.methods[i].name);
								fprintf(processed_header_file, "%s%s%s_%s(unsigned type, ...);\n", current_class.methods[i].type, PROCESSED_METHOD_PREFIX, lwname, current_class.methods[i].name);
								fprintf(processed_file, "unsigned __type__ = %d;\n", import_struct.class_count-1);
								fprintf(processed_file, "(void)__type__;\n");
								fprintf(processed_file, "va_list args;\n");
								fprintf(processed_file, "va_start(args, type);\n");
								for (size_t j = 0; j < current_class.methods[i].arguments_count; j++) {
									fprintf(processed_file, "%s %s = va_arg(args, %s);\n", current_class.methods[i].arguments[j].type, current_class.methods[i].arguments[j].name, current_class.methods[i].arguments[j].type);
								}
								fprintf(processed_file, "va_end(args);\n");
								fprintf(processed_file, "(void)this;\n");
								
								fprintf(processed_file, "%s\n", current_class.methods[i].content);

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

						}
					}
				} else {
					if (strstr(line, "#")) {
						if (strstr(line, "#include \"")) {
							fprintf(processed_file, "#include \"../../%s", line + strlen("#include \""));
						} else {
							fprintf(processed_file, "%s", line);
						}
					}
					in_class = get_class_name(line, &current_class);
					if (in_class) {

						char *extends = strstr(line, "extends");
						if (extends) {
							extends += strlen("extends");
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
						printf("Class name: %s\n", current_class.name);
					}
				}


			} while (!feof(source_file));
			fprintf(processed_header_file, "#endif\n");
			fclose(source_file);
			fclose(processed_file);
			fclose(processed_header_file);
		}
		if (need_rewriting_imports) {

			char *filepath = malloc(strlen(CLASS_PATH) + strlen(CLASS_IMPORT_HEADER_FILENAME));
			filepath[0] = 0;
			strcat(filepath, CLASS_PATH);
			strcat(filepath, CLASS_IMPORT_HEADER_FILENAME);
			FILE * import_class_header_file = fopen(filepath, "w");
			free(filepath);

			fprintf(import_class_header_file, "// This file is generated by the class_tools.c tool\n");
			for (int i = 0; i < n; i++) {
				fprintf(import_class_header_file, "#include \"%s\"\n", import_struct.filepath[i]);
			}
			fprintf(import_class_header_file, "#ifndef IMPORT_CLASS_H\n");
			fprintf(import_class_header_file, "#define IMPORT_CLASS_H\n");
			fprintf(import_class_header_file, "typedef enum ClassType {\n");
			for (size_t i = 0; i < import_struct.class_count; i++) {
				char uppername[NAME_SIZE];
				strcpy(uppername, import_struct.class_name[i]);
				fprintf(import_class_header_file, "\t%s%s,\n", CLASS_TYPE_PREFIX, uppercase_string(uppername));
			}
			fprintf(import_class_header_file, "\t%sCOUNT\n", CLASS_TYPE_PREFIX);
			fprintf(import_class_header_file, "} ClassType;\n");
			fprintf(import_class_header_file, "struct MethodsCorrespondance {\n");
			for (size_t i = 0; i < import_struct.unique_method_count; i++) {
				fprintf(import_class_header_file, "\t%s (*%s[%d])(unsigned type, ...);\n", import_struct.unique_method_type[i], import_struct.unique_method_name[i], import_struct.class_count);
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
			for (size_t i = 0; i < import_struct.unique_method_count; i++){
				fprintf(import_class_header_file, "\t\t.%s = {", import_struct.unique_method_name[i]);
				for (size_t j = 0; j < import_struct.class_count; j++) {
					fprintf(import_class_header_file, "%s", import_struct.corresponding_method[i][j]);
					if (j < import_struct.class_count - 1) fprintf(import_class_header_file, ", ");
				}
				fprintf(import_class_header_file, "},\\\n");
			}
			fprintf(import_class_header_file, "\t},\\\n");
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
			
			fclose(import_class_header_file);
		}
		free(namelist);
	}
}