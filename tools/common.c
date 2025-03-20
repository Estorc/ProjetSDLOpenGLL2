#include <ctype.h>

time_t getFileCreationTime(char *path) {
    struct stat attr;
    stat(path, &attr);
    if (stat(path, &attr) == 0) { // Check if stat is successful
        return attr.st_mtime;
    } else {
        return 0;
    }
}


int compareFiles(FILE *fp1, FILE *fp2) {
    if (!fp1 || !fp2) {
        perror("Invalid file pointers");
        return -1; // Error: NULL file pointers
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

    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    return 0;  // Success
}

#ifdef _WIN32
#define MAX_TEMP_FILES 100  // Limit on the number of temp files

static char *tempFilePaths[MAX_TEMP_FILES] = {0};  // Array to store paths of temp files
static int tempFileCount = 0;  // Counter to track the number of temporary files

void cleanupTempFiles() {
    for (int i = 0; i < tempFileCount; i++) {
        if (tempFilePaths[i] != NULL) {
            if (DeleteFileA(tempFilePaths[i]) == 0) {
                fprintf(stderr, "Failed to delete temp file: %s\n", tempFilePaths[i]);
            }
            free(tempFilePaths[i]);  // Free the dynamically allocated path memory
            tempFilePaths[i] = NULL;
        }
    }
}
#else
void cleanupTempFiles() {
    // No cleanup needed on non-Windows platforms
}
#endif

FILE * createTempFile() {
	#ifdef _WIN32

	char tempPath[MAX_PATH];
	char tempFile[MAX_PATH];

	GetTempPathA(MAX_PATH, tempPath);  // Obtenir le répertoire temporaire
	GetTempFileNameA(tempPath, "tmp", 0, tempFile);  // Créer un fichier temporaire

    // Store the path in the array (dynamically allocate memory for each path)
    if (tempFileCount < MAX_TEMP_FILES) {
        tempFilePaths[tempFileCount] = malloc(strlen(tempFile) + 1);
        if (tempFilePaths[tempFileCount] != NULL) {
            strcpy(tempFilePaths[tempFileCount], tempFile);
            tempFileCount++;
        }
    }

	return fopen(tempFile, "w+b");
	#else
	return tmpfile();
	#endif
}


void clear_spaces(char *str) {
	char *i = str;
	char *j = str;
	while (*j != 0) {
		*i = *j++;
		if (!isspace((unsigned char)*i)) {
			i++;
		}
	}
	*i = 0;
}

void trim_spaces(char* str) {
    // Pointer to the start of the string
    char *start = str;
    // Move the start pointer to the first non-space character
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // If the string is not empty, move the string to start from the first non-space character
    if (*start == '\0') {
        *str = '\0';  // If the string consists only of spaces, set it to empty
        return;
    }

    // Pointer to the end of the string
    char *end = str + strlen(str) - 1;

    // Move the end pointer to the last non-space character
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Null-terminate the string after the last non-space character
    *(end + 1) = '\0';

    // Move the string to the start pointer location
    memmove(str, start, end - start + 2);
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