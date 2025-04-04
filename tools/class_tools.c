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

#include "class_tools_config.h"

#ifdef _WIN32
#include "class_tools_win.h"
#else
#include "class_tools_linux.h"
#endif


#include "common.c"
#include "../src/io/stringio.c"


#pragma region Structs
/**
 * ==================================================================================
 * STRUCTS
 * ==================================================================================
 */


typedef struct Argument {
	char *name;
	char *type;
} Argument;

typedef struct Method {
	char *name;
	char *type;
	Argument *arguments;
	int arguments_count;
} Method;

typedef struct Class {
	char *name;
	char *type;
	Method *methods;
	int methods_count;
} Class;

typedef struct ImportStruct {
	Class **classes;
	char **unique_method_name;
	char **class_name;
	char **extends;
	char **type;
	char **filepath;
	char **srcfilepath;
	int type_count;
	int class_count;
	int unique_method_count;
} ImportStruct;

typedef enum {
	OUTSIDE,
	CLASS,
	METHOD,
	STATIC_METHOD,
	RETURN
} Scope;


#pragma region Globals
/**
 * ==================================================================================
 * GLOBALS
 * ==================================================================================
 */

FILE * source_file;
FILE * processed_file;
FILE * temp_processed_header_file;
FILE * processed_header_file;

#pragma region Functions
/**
 * ==================================================================================
 * FUNCTIONS
 * ==================================================================================
 */


ImportStruct *create_import_struct(int n) {
	ImportStruct *import_struct = malloc(sizeof(ImportStruct));

	memset(import_struct, 0, sizeof(ImportStruct));

	import_struct->filepath 			= malloc(sizeof(char *) * n);
	import_struct->srcfilepath 			= malloc(sizeof(char *) * n);

	import_struct->unique_method_name 	= malloc(sizeof(char *));
	import_struct->type 				= malloc(sizeof(char *) * 2);
	import_struct->class_name 			= malloc(sizeof(char *));
	import_struct->extends 				= malloc(sizeof(char *));

	import_struct->class_count = 0;
	import_struct->unique_method_count = 0;

	import_struct->type_count = 1;
	import_struct->type[0] = strdup("void");

	return import_struct;
}

void free_import_struct(ImportStruct **import_struct) {
	free(*import_struct);
	*import_struct = NULL;
}


Class * get_current_class(ImportStruct * import_struct) {
	return import_struct->classes[import_struct->class_count-1];
}

void create_wrapper_function(char * type, int type_index, FILE * src_file, FILE * header_file) {
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


void apply_promotion(char **type) {
	const char *promotions[] = {
		"int", "int",
		"float", "double",
		"char", "int",
		"short", "int",
		"long", "long",
		"double", "double",
		"void", "void",
	
		"unsigned int", "unsigned long",
		"unsigned long", "unsigned long",
		"unsigned char", "unsigned int",
		"unsigned short", "unsigned int",
		"unsigned double", "unsigned double",
		"unsigned float", "unsigned double",
		"unsigned void", "unsigned void",
	
		"vec2", "float *",
		"vec3", "float *",
		"vec4", "float *",
		"mat2", "float **",
		"mat2x3", "float **",
		"mat2x4", "float **",
		"mat3", "float **",
		"mat3x2", "float **",
		"mat3x4", "float **",
		"mat4", "float (*)[4]",
		"mat4x2", "float **",
		"mat4x3", "float **",
		"vec2i", "int *",
		"vec3i", "int *",
		"vec4i", "int *",
	
		"u8", "int",
		"u16", "int",
		"u32", "long",
		"u64", "long",
		"s8", "int",
		"s16", "int",
		"s32", "long",
		"s64", "long",
		"f32", "double",
		"f64", "double",
		"bool", "int",
		"size_t", "long"
	};

	for (int i = 0; i < sizeof(promotions) / sizeof(promotions[0]); i += 2) {
		if (strcmp(*type, promotions[i]) == 0) {
			free(*type);
			*type = strdup(promotions[i + 1]);
			break;
		}
	}
}


void apply_suffix(char **type, char **name) {
	if (strrchr(*type, ')')) {
		char *suffix = strrchr(*type, ')');
		*suffix = '\0';
		*name = realloc(*name, sizeof(char) * (strlen(*name) + strlen(suffix + 1) + 1));
		sprintf(*name, "%s)%s", *name, suffix + 1);
	} else if (strrchr(*type, '[')) {
		char *suffix = strrchr(*type, '[');
		*suffix = '\0';
		*name = realloc(*name, sizeof(char) * (strlen(*name) + strlen(suffix + 1) + 1));
		sprintf(*name, "%s[%s", *name, suffix + 1);
	}
}


int get_method_args(char *args_str, Argument **args) {
	char *arg_start = args_str;
	char *arg_end = args_str;
	int arg_count = 1;
	while (*arg_end) {
		arg_end++;
		if (*arg_end == ',' || *arg_end == '\0') {

			*args = realloc(*args, sizeof(Argument) * (arg_count + 1));
			if (!*args) {
				perror("realloc");
				return -1;
			}
			Argument *arg = *args;

			while (*arg_start == ' ') arg_start++;
			char *argstr = malloc(sizeof(char) * (arg_end - arg_start + 1));
			strncpy(argstr, arg_start, arg_end - arg_start);
			argstr[arg_end - arg_start] = '\0';

			if (strstr(argstr, "...")) {
				return arg_count;
			}
			
			char * argstr_end = argstr + strlen(argstr);
			while (*argstr_end == ' ') argstr_end--;
			while (*argstr_end != ' ' && *argstr_end != '*') argstr_end--;
			

			arg[arg_count].name = malloc(sizeof(char) * strlen(argstr));
			arg[arg_count].type = malloc(sizeof(char) * strlen(argstr));

			strcpy(arg[arg_count].name, argstr_end + 1);
			strncpy(arg[arg_count].type, argstr, argstr_end - argstr + 1);
			arg[arg_count].type[argstr_end - argstr + 1] = '\0';

			trim_spaces(arg[arg_count].type);
			apply_promotion(&arg[arg_count].type);
			apply_suffix(&arg[arg_count].type, &arg[arg_count].name);

			arg[arg_count].type = realloc(arg[arg_count].type, sizeof(char) * (strlen(arg[arg_count].type) + 1));

			free(argstr);
			printf("\t\tFound argument (%s) [%s]\n", arg[arg_count].type, arg[arg_count].name);
			
			arg_count++;
			arg_start = arg_end + 1;
		}
	}
	return arg_count;
}



void write_and_save_method(char *line, Method *method, ImportStruct *import_struct, int file_index, int line_count) {
	Class *current_class = get_current_class(import_struct);
	current_class->methods = realloc(current_class->methods, sizeof(Method) * (current_class->methods_count + 2));
	if (!current_class->methods) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	Method *current_method = &current_class->methods[current_class->methods_count];
	clear_spaces(current_method->type);
	int type_index = find_string_index(current_method->type, (const char * const*)import_struct->type, import_struct->type_count);
	if (type_index == -1 && current_method->type) {
		import_struct->type[import_struct->type_count] = strdup(current_method->type);
		type_index = import_struct->type_count;
		create_wrapper_function(current_method->type, type_index, processed_file, temp_processed_header_file);
		import_struct->type_count++;
		import_struct->type = realloc(import_struct->type, sizeof(char *) * (import_struct->type_count + 1));
	}
	fprintf(processed_file, "#line %d \"%s\"\n", line_count, import_struct->srcfilepath[file_index]);
	current_method->arguments[0].name = strdup("this");
	current_method->arguments[0].type = strdup(current_class->type);

	char *lwname = strdup(current_class->name);
	lowercase_string(lwname);
	fprintf(processed_file, "void %s%s_%s(void * %s, va_list args) {", PROCESSED_METHOD_PREFIX, lwname, current_method->name, RETURN_VOID_POINTER);
	free(lwname);
	if (strcmp(current_method->type, "void") != 0) {
		fprintf(processed_file, "%s * %s = (%s *) %s;", current_method->type, RETURN_POINTER, current_method->type, RETURN_VOID_POINTER);
	}
	for (size_t j = 0; j < current_method->arguments_count; j++) {
		char * arg_type_end = current_method->arguments[j].name + strspn(current_method->arguments[j].name, INSTRUCTION_CHARSET);
		fprintf(processed_file, "%s %s = va_arg(args, %s%s);", current_method->arguments[j].type, current_method->arguments[j].name, current_method->arguments[j].type, arg_type_end);
	}
	fprintf(processed_file, "(void)this;\n");
}


typedef enum {
	METHOD_NORMAL,
	METHOD_STATIC,
	METHOD_NONE = -1
} MethodType;

int get_class_method(char *line, Method *method, ImportStruct *import_struct, int file_index, int line_count) {
	while (*line == ' ' || *line == '\t') {
		line++;
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
			static char func_args[1024];
			strncpy(func_args, expr_pos+1, func_args_len);
			func_args[func_args_len] = '\0';


			char *methodstr = strdup(line);
			methodstr[expr_len] = '\0';
			
			char * methodstr_end = methodstr + strlen(methodstr);
			while (*methodstr_end == ' ') methodstr_end--;
			while (*methodstr_end != ' ' && *methodstr_end != '*') methodstr_end--;

			method->name = malloc(sizeof(char) * (expr_len + 1));
			strncpy(method->name, methodstr_end + 1, expr_len);
			method->type = malloc(sizeof(char) * (methodstr_end - methodstr + 1));
			strncpy(method->type, methodstr, methodstr_end - methodstr + 1);

			method->name[expr_len] = '\0';
			method->type[methodstr_end - methodstr] = '\0';

			free(methodstr);

			if (strstr(method->type, "static")) {
				free(method->type);
				free(method->name);
				return METHOD_STATIC;
			}

			printf("\tFound method (%s) %s\n", method->type, method->name);

			method->arguments = malloc(sizeof(Argument) * 1);

			if (func_args_len) method->arguments_count = get_method_args(func_args, &method->arguments);
			else method->arguments_count = 1;



			write_and_save_method(line, method, import_struct, file_index, line_count);


			return METHOD_NORMAL;
		}
	}

	return METHOD_NONE;
}


void write_and_save_class(char *line, Class *class, ImportStruct *import_struct) {
	char *name = strdup(class->name);
	uppercase_string(name);

	import_struct->classes = realloc(import_struct->classes, sizeof(Class *) * (import_struct->class_count + 1));
	import_struct->classes[import_struct->class_count] = class;

	fprintf(processed_file, "static unsigned __type__ __attribute__((unused)) = %s%s;", CLASS_TYPE_PREFIX, name);

	char *extends = strstr(line, ": public");
	if (extends) {
		extends += strlen(": public");
		extends += strspn(extends, " ");
		size_t extends_len = strspn(extends, INSTRUCTION_CHARSET);
		import_struct->extends[import_struct->class_count] = malloc(sizeof(char) * (extends_len + 1));
		strncpy(import_struct->extends[import_struct->class_count], extends, extends_len);
		import_struct->extends[import_struct->class_count][extends_len] = '\0';
	} else {
		import_struct->extends[import_struct->class_count] = NULL;
	}

	class->methods_count = 0;
	
	import_struct->class_name[import_struct->class_count] = strdup(class->name);
	import_struct->class_count++;
	import_struct->class_name = realloc(import_struct->class_name, sizeof(char *) * (import_struct->class_count + 1));
	import_struct->extends = realloc(import_struct->extends, sizeof(char *) * (import_struct->class_count + 1));

	free(name);
}


bool get_class_name(char *line, Class **class, ImportStruct *import_struct) {
	char *expr_pos = line + strspn(line, " ");
	if (expr_pos != strstr(expr_pos, CLASS_PREFIX)) return false;
	if (expr_pos) {
		(*class) = malloc(sizeof(Class));
		memset(*class, 0, sizeof(Class));
		size_t expr_len = strlen(line);
		expr_pos += strlen(CLASS_PREFIX);
		expr_pos += strspn(expr_pos, " ");
		expr_len = strspn(expr_pos, INSTRUCTION_CHARSET);
		(*class)->name = malloc(sizeof(char) * (expr_len + 1));
		strncpy((*class)->name, expr_pos, expr_len);
		(*class)->name[expr_len] = '\0';
		(*class)->methods = malloc(sizeof(Method) * 1);
		printf("Found class %s\n", (*class)->name);
		write_and_save_class(line, *class, import_struct);
		return true;
	}
	return false;
}

void recursive_scan(char *path, char *base_path, dirent_t ***namelist, int (*filter)(const dirent_t *entry), int *n) {
	if (base_path == NULL) base_path = path;
	dirent_t **local_namelist;
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
			*namelist = realloc(*namelist, (*n + 1) * sizeof(dirent_t *));
			(*namelist)[*n] = malloc(sizeof(dirent_t));
			memcpy((*namelist)[*n], local_namelist[i], sizeof(dirent_t));
			if (strlen(new_path))
				snprintf((*namelist)[*n]->d_name, sizeof((*namelist)[*n]->d_name), "%s/%s", new_path + 1, local_namelist[i]->d_name);
			else
				snprintf((*namelist)[*n]->d_name, sizeof((*namelist)[*n]->d_name), "%s", local_namelist[i]->d_name);
			(*n)++;
		}
	}
	free(local_namelist);
}


char * get_line(FILE * file) {
	char * line;
	int j = 0;
	int line_length = 128;
	line = malloc(sizeof(char) * (line_length + 1));
	for (; line[j-1] != '\n' && !feof(file); j++) {
		if (j >= line_length) line_length <<= 1;
		line = realloc(line, sizeof(char) * (line_length + 1));
		line[j] = fgetc(file);
	}
	line[j] = 0;
	if (feof(file)) line[j-1] = 0;
	return line;
}


void exit_class(ImportStruct * import_struct) {
	Class *current_class = get_current_class(import_struct);
	char *name = strdup(current_class->name);
	lowercase_string(name);

	for (size_t i = 0; i < current_class->methods_count; i++) {
		fprintf(temp_processed_header_file, "void %s%s_%s(void * %s, va_list args);\n", PROCESSED_METHOD_PREFIX, name, current_class->methods[i].name, RETURN_VOID_POINTER);

		int index;
		if ((index = find_string_index(current_class->methods[i].name, (const char * const*)import_struct->unique_method_name, import_struct->unique_method_count)) == -1) {
			import_struct->unique_method_name[import_struct->unique_method_count] = strdup(current_class->methods[i].name);
			index = import_struct->unique_method_count;
			import_struct->unique_method_count++;
			import_struct->unique_method_name = realloc(import_struct->unique_method_name, sizeof(char *) * (import_struct->unique_method_count + 1));
		}
	}
	free(name);
}

dirent_t ** get_files(int *files_count) {
	dirent_t **namelist = NULL;
	int n = 0;
	int filter(const dirent_t *entry) {
		return (strstr(entry->d_name, ".class.c") != NULL && !strstr(entry->d_name, PROCESSED_PREFIX)) || (entry->d_type == DT_DIR && entry->d_name[0] != '.' && entry->d_name[0] != '_');
	}

	char dirpath[PATH_SIZE];
	strcpy(dirpath, SRC_PATH);
	strcat(dirpath, PROCESSED_PREFIX);
	mkdir(dirpath, 0755);
	recursive_scan(SRC_PATH, NULL, &namelist, filter, &n);


	if (n < 0) {
		perror("scandir");
		exit(EXIT_FAILURE);
	}

	printf("Found %d files\n", n);
	*files_count = n;
	return namelist;
}


char *open_files(ImportStruct *import_struct, dirent_t **fetched_file_info, int file_index) {
	char *filepath = malloc(strlen(fetched_file_info[file_index]->d_name) + strlen(SRC_PATH) + 1);
	filepath[0] = 0;
	strcat(filepath, SRC_PATH);
	strcat(filepath, fetched_file_info[file_index]->d_name);
	source_file = fopen(filepath, "r");
	if (!source_file) {
		fprintf(stderr, "Le fichier \"%s\" n'existe pas!", filepath);
		exit(EXIT_FAILURE);
	}

	time_t lastEditTime = getFileCreationTime(filepath);
	import_struct->srcfilepath[file_index] = strdup(filepath);

	filepath = realloc(filepath, strlen(fetched_file_info[file_index]->d_name) + strlen(SRC_PATH) + strlen(PROCESSED_PREFIX) + 1);
	filepath[0] = 0;
	strcat(filepath, SRC_PATH);
	strcat(filepath, PROCESSED_PREFIX);
	replace_char(fetched_file_info[file_index]->d_name, '/', '_');
	strcat(filepath, fetched_file_info[file_index]->d_name);

	time_t lastProcessedTime = 0;
	lastProcessedTime = getFileCreationTime(filepath);

	if (lastEditTime < lastProcessedTime) {
		processed_file = fopen(VOID_FILE, "w+b");
		processed_header_file = NULL;
		if (processed_file == NULL) {
			exit(EXIT_FAILURE);
		}
		filepath[strlen(filepath) - 1] = 'h';
	} else {
		processed_file = fopen(filepath, "w+b");
		printf("Processing file -> %s\n", filepath);
		filepath[strlen(filepath) - 1] = 'h';
		processed_header_file = fopen(filepath, "r+b");
		if (!processed_header_file) {
			processed_header_file = fopen(filepath, "w+b");	
		}
	}

	temp_processed_header_file = createTempFile();

	import_struct->filepath[file_index] = strdup(filepath + strlen(SRC_PATH));

	return filepath;
}

void close_files() {
	fclose(source_file);
	fclose(processed_file);
	fclose(temp_processed_header_file);
	if (processed_header_file != NULL) fclose(processed_header_file);
	cleanupTempFiles();
}


char * get_filename(dirent_t **fetched_file_info, int file_index) {
	size_t filename_len = strspn(fetched_file_info[file_index]->d_name, INSTRUCTION_CHARSET);
	char *filename = malloc(sizeof(char) * (filename_len + 1));
	strncpy(filename, fetched_file_info[file_index]->d_name, filename_len);
	filename[filename_len] = '\0';
	uppercase_string(filename);
	return filename;
}

void add_file_to_imports(char * filename) {
	fprintf(temp_processed_header_file, "#ifndef %s%s_H\n", PROCESSED_HEADER_PREFIX, filename);
	fprintf(temp_processed_header_file, "#define %s%s_H\n", PROCESSED_HEADER_PREFIX, filename);
}

void write_class_needed_headers() {
	fprintf(processed_file, "#include <stdarg.h>\n");
}

bool catch_opening_brace(char *line, int *brace_count) {
	if (strstr(line, "{")) {
		(*brace_count)++;
		return true;
	}
	return false;
}

bool catch_closing_brace(char *line, int *brace_count) {
	if (strstr(line, "}")) {
		(*brace_count)--;
		return true;
	}
	return false;
}

bool exited_section(int brace_count, Scope current_scope) {
	return (current_scope == CLASS) ? brace_count <= -1 : brace_count <= 0;
}

bool catch_preprocessor_directive(char *line) {
	if (strstr(line, "#")) {
		if (strstr(line, "#include \"")) {
			fprintf(processed_file, "#include \"../%s", line + strlen("#include \""));	// ../ because we are in the __processed__ directory
			fprintf(temp_processed_header_file, "#include \"../src/%s", line + strlen("#include \"")); // ../src because we are in the __processed__ directory
		} else {
			fprintf(processed_file, "%s", line);
			fprintf(temp_processed_header_file, "%s", line);
		}
		return true;
	}
	return false;
}

bool catch_return(char **line, Scope *current_scope) {
	char * return_pos;
	if ((return_pos = strstr(*line, "return "))) {
		(*line)[return_pos - *line] = 0;
		fprintf(processed_file, "%s", *line);
		*line = return_pos;
		*line += strlen("return ");
		fprintf(processed_file, "{*%s = ", RETURN_POINTER);
		*current_scope = RETURN;
		return true;
	}
	return false;
}

void write_return(char *line, Scope *current_scope) {
	while (*line) {
		if (*line == ';') {
			fprintf(processed_file, ";return;}\n");
			*current_scope = METHOD;
			break;
		} else fputc(*line, processed_file);
		line++;
	}
}

void write_line(char *line) {
	fprintf(processed_file, "%s", line);
}

bool fetch_private(char *line) {
	if (strstr(line, "private:")) {
		return true;
	}
	return false;
}

bool fetch_public(char *line) {
	if (strstr(line, "public:")) {
		return true;
	}
	return false;
}

bool fetch_class_type(char *line, Class *class) {
	if (strstr(line, CONTAINER_TYPE_PREFIX)) {
		char *expr_pos = strstr(line, CONTAINER_TYPE_PREFIX);
		expr_pos += strlen(CONTAINER_TYPE_PREFIX);
		expr_pos += strspn(expr_pos, " ");
		size_t expr_len = strcspn(expr_pos, "\n");
		class->type = malloc(sizeof(char) * (expr_len + 1));
		strncpy(class->type, expr_pos, expr_len);
		class->type[expr_len] = '\0';
		return true;
	}
	return false;
}

int fetch_file(ImportStruct *import_struct, dirent_t **fetched_file_info, int file_index) {

	bool need_rewriting_imports = false;
	int brace_count = 0;
	int line_count = 0;
	Scope current_scope = OUTSIDE;
	Class *current_class;

	char * filepath = open_files(import_struct, fetched_file_info, file_index);
	char * filename = get_filename(fetched_file_info, file_index);

	add_file_to_imports(filename);
	write_class_needed_headers();

	do {
		line_count++;
		char *line = get_line(source_file);

		if (current_scope && current_scope != RETURN) {
			catch_opening_brace(line, &brace_count);
			if (catch_closing_brace(line, &brace_count)) {
				if (exited_section(brace_count, current_scope)) {
					switch (current_scope) {
						case METHOD:
							current_class->methods_count++;
						case STATIC_METHOD:
							current_scope = CLASS;
						break;
						case CLASS:
							brace_count = 0;
							exit_class(import_struct);
							continue;
						break;
						default:
							fprintf(processed_file, "%s", line);
						break;
					}
				}
			}
		}

		switch (current_scope) {
			case OUTSIDE:
				catch_preprocessor_directive(line);
				if (get_class_name(line, &current_class, import_struct)) current_scope = CLASS;
				break;
			break;
			case METHOD:
				if (!catch_return(&line, &current_scope)) {
					write_line(line);
					break;
				}
			case RETURN:
				write_return(line, &current_scope);
			break;
			case STATIC_METHOD:
				write_line(line);
			break;
			case CLASS:
				if (fetch_private(line)) continue;
				if (fetch_public(line)) continue;
				if (fetch_class_type(line, current_class)) continue;
				switch (get_class_method(line, &current_class->methods[current_class->methods_count], import_struct, file_index, line_count)) {
					case METHOD_STATIC:
						write_line(line);
						current_scope = STATIC_METHOD;
					break;
					case METHOD_NONE:
						write_line(line);
					break;
					default:
						current_scope = METHOD;
					break;
				}
			break;
			default:
				fprintf(stderr, "Unknown scope\n");
			break;
		}
	} while (!feof(source_file));

	fprintf(temp_processed_header_file, "#endif\n");
	rewind(temp_processed_header_file);
	if (processed_header_file != NULL && compareFiles(temp_processed_header_file, processed_header_file)) {
		printf("Changes detected in header file\n");
		rewind(temp_processed_header_file);
		processed_header_file = freopen(filepath, "wb", processed_header_file);
		copyFile(temp_processed_header_file, processed_header_file);
		need_rewriting_imports = true;
	}
	free(filepath);
	close_files();

	return need_rewriting_imports;
}

void write_correspondances(ImportStruct *import_struct, int files_count) {

	printf("Writing correspondances\n");
	char uppername[NAME_SIZE];
	char *filepath = malloc(strlen(CLASS_PATH) + strlen(CLASS_IMPORT_HEADER_FILENAME));
	filepath[0] = 0;
	strcat(filepath, CLASS_PATH);
	strcat(filepath, CLASS_IMPORT_HEADER_FILENAME);
	FILE * json_data_file = fopen("./tools/import_classes.json", "w");
	FILE * import_classes_header_file = fopen(filepath, "w");
	free(filepath);

	filepath = malloc(strlen(CLASS_PATH) + strlen(CLASS_TYPE_HEADER_FILENAME));
	filepath[0] = 0;
	strcat(filepath, CLASS_PATH);
	strcat(filepath, CLASS_TYPE_HEADER_FILENAME);
	FILE * class_type_header_temp_file = createTempFile();
	FILE * class_type_header_file = fopen(filepath, "r");

	fprintf(import_classes_header_file, "// This file is generated by the class_tools.c tool\n");
	for (int i = 0; i < files_count; i++) {
		fprintf(import_classes_header_file, "#include \"../%s\"\n", import_struct->filepath[i]);
	}
	fprintf(import_classes_header_file, "#ifndef IMPORT_CLASS_H\n");
	fprintf(import_classes_header_file, "#define IMPORT_CLASS_H\n");
	fprintf(class_type_header_temp_file, "#pragma once\n");
	fprintf(class_type_header_temp_file, "typedef enum ClassType {\n");
	fprintf(json_data_file, "{\n");
	fprintf(json_data_file, "\t\"type_associations\": {\n");
	for (size_t i = 0; i < import_struct->class_count; i++) {
		strcpy(uppername, import_struct->class_name[i]);
		fprintf(json_data_file, "\t\t\"%s\": \"%s%s\"%c\n", import_struct->class_name[i], CLASS_TYPE_PREFIX, uppercase_string(uppername), i < import_struct->class_count - 1 ? ',' : ' ');
		fprintf(class_type_header_temp_file, "\t%s%s,\n", CLASS_TYPE_PREFIX, uppercase_string(uppername));
	}
	fprintf(json_data_file, "\t},\n");
	fprintf(class_type_header_temp_file, "\t%sCOUNT\n", CLASS_TYPE_PREFIX);
	fprintf(class_type_header_temp_file, "} ClassType;\n");
	fprintf(import_classes_header_file, "struct MethodsCorrespondance {\n");
	for (size_t i = 0; i < import_struct->unique_method_count; i++) {
		fprintf(import_classes_header_file, "\tvoid (*%s[%d])(void * %s, va_list args);\n", import_struct->unique_method_name[i], import_struct->class_count, RETURN_VOID_POINTER);
	}
	fprintf(import_classes_header_file, "};\n");

	fprintf(import_classes_header_file, "struct ClassManager {\n");
	fprintf(import_classes_header_file, "\tstruct MethodsCorrespondance methodsCorrespondance;\n");
	fprintf(import_classes_header_file, "\tClassType extends[%d];\n", import_struct->class_count);
	fprintf(import_classes_header_file, "\tconst char * class_names[%d];\n", import_struct->class_count);
	fprintf(import_classes_header_file, "};\n");

	fprintf(import_classes_header_file, "#define BUILD_CLASS_METHODS_CORRESPONDANCE(classManager) const struct ClassManager classManager = {\\\n");
	fprintf(import_classes_header_file, "\t.methodsCorrespondance = {\\\n");
	fprintf(json_data_file, "\t\"method_index\": {\n");
	for (size_t i = 0; i < import_struct->unique_method_count; i++){
		fprintf(json_data_file, "\t\t\"%s\": %ld%c\n", import_struct->unique_method_name[i], i, i < import_struct->unique_method_count - 1 ? ',' : ' ');
		fprintf(import_classes_header_file, "\t\t.%s = {", import_struct->unique_method_name[i]);
		for (size_t j = 0; j < import_struct->class_count; j++) {
			size_t class_index = j;
			size_t extends_index = find_string_index(import_struct->extends[class_index], (const char * const*)import_struct->class_name, import_struct->class_count);
			char **method_name_array = malloc(sizeof(char *) * import_struct->classes[class_index]->methods_count);
			for (size_t k = 0; k < import_struct->classes[class_index]->methods_count; k++) {
				method_name_array[k] = import_struct->classes[class_index]->methods[k].name;
			}
			while ((find_string_index(import_struct->unique_method_name[i], (const char * const*)method_name_array, import_struct->classes[class_index]->methods_count)) == -1 && extends_index != -1) {
				class_index = extends_index;
				extends_index = find_string_index(import_struct->extends[class_index], (const char * const*)import_struct->class_name, import_struct->class_count);
				method_name_array = realloc(method_name_array, sizeof(char *) * import_struct->classes[class_index]->methods_count);
				for (size_t k = 0; k < import_struct->classes[class_index]->methods_count; k++) {
					method_name_array[k] = import_struct->classes[class_index]->methods[k].name;
				}
			}
			int method_index = find_string_index(import_struct->unique_method_name[i], (const char * const*)method_name_array, import_struct->classes[class_index]->methods_count);
			if (method_index == -1) fprintf(import_classes_header_file, "NULL");
			else {
				char *name = strdup(import_struct->class_name[class_index]);
				fprintf(import_classes_header_file, "%s%s_%s", PROCESSED_METHOD_PREFIX, lowercase_string(name), import_struct->classes[class_index]->methods[method_index].name);
				free(name);
			}
			if (j < import_struct->class_count - 1) {
				fprintf(import_classes_header_file, ", ");
			}
			free(method_name_array);
		}
		fprintf(import_classes_header_file, "},\\\n");
	}
	fprintf(json_data_file, "\t},\n");
	fprintf(import_classes_header_file, "\t},\\\n");

	fprintf(json_data_file, "\t\"type_caller\": {\n");
	for (size_t i = 0; i < import_struct->class_count; i++){
		fprintf(json_data_file, "\t\t\"%s\": [", import_struct->class_name[i]);
		for (size_t j = 0; j < import_struct->unique_method_count; j++) {
			char **method_name_array = malloc(sizeof(char *) * import_struct->classes[i]->methods_count);
			for (size_t k = 0; k < import_struct->classes[i]->methods_count; k++) {
				method_name_array[k] = import_struct->classes[i]->methods[k].name;
			}
			int type_id = 0;
			int index = find_string_index(import_struct->unique_method_name[j], (const char * const*)method_name_array, import_struct->classes[i]->methods_count);
			if (index != -1) type_id = find_string_index(import_struct->classes[i]->methods[index].type, (const char * const*)import_struct->type, import_struct->type_count);
			fprintf(json_data_file, "\"%s_%d\"%c", CALLER_NAME, type_id, j < import_struct->unique_method_count - 1 ? ',' : ' ');
			free(method_name_array);
		}
		fprintf(json_data_file, "]%c\n", i < import_struct->class_count - 1 ? ',' : ' ');
	}
	fprintf(json_data_file, "\t},\n");

	fprintf(json_data_file, "\t\"type_cast\": {\n");
	for (size_t i = 0; i < import_struct->type_count; i++){
		fprintf(json_data_file, "\t\t\"%s\": \"%s_%ld\"%c\n", import_struct->type[i], CALLER_NAME, i, i < import_struct->type_count - 1 ? ',' : ' ');
	}
	fprintf(json_data_file, "\t}\n");


	fprintf(import_classes_header_file, "\t.extends = {");
	for (size_t i = 0; i < import_struct->class_count; i++) {
		fprintf(import_classes_header_file, "%d", find_string_index(import_struct->extends[i], (const char * const*)import_struct->class_name, import_struct->class_count));
		if (i < import_struct->class_count - 1) fprintf(import_classes_header_file, ", ");
	}
	fprintf(import_classes_header_file, "},\\\n");
	fprintf(import_classes_header_file, "\t.class_names = {");
	for (size_t i = 0; i < import_struct->class_count; i++) {
		fprintf(import_classes_header_file, "\"%s\"", import_struct->class_name[i]);
		if (i < import_struct->class_count - 1) fprintf(import_classes_header_file, ", ");
	}
	fprintf(import_classes_header_file, "}\\\n");
	fprintf(import_classes_header_file, "};\n");
	
	fprintf(import_classes_header_file, "#endif\n");
	fprintf(json_data_file, "}\n");
	
	fclose(json_data_file);
	fclose(import_classes_header_file);

	rewind(class_type_header_temp_file);
	if (compareFiles(class_type_header_temp_file, class_type_header_file)) {
		rewind(class_type_header_temp_file);
		class_type_header_file = freopen(filepath, "wb", class_type_header_file);
		copyFile(class_type_header_temp_file, class_type_header_file);
	}
	fclose(class_type_header_temp_file);
	free(filepath);
	cleanupTempFiles();
}


bool fetch_all_files(ImportStruct *import_struct, dirent_t **namelist, int files_count) {
	bool need_rewriting_imports = false;
	for (int i = 0; i < files_count; i++) {
		need_rewriting_imports |= fetch_file(import_struct, namelist, i);
	}
	return need_rewriting_imports;
}

#pragma region Main
/**
 * ==================================================================================
 * MAIN
 * ==================================================================================
 */

int main(int argc, char ** argv) {
	int n;
	dirent_t **namelist = get_files(&n);
	ImportStruct *import_struct = create_import_struct(n);
	if (fetch_all_files(import_struct, namelist, n)) write_correspondances(import_struct, n);
	free(namelist);
	free_import_struct(&import_struct);
	printf("Done\n");
	return 0;
}