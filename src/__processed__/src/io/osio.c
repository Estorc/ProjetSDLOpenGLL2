#line 1 "src/io/osio.c"
#include <stdio.h>
#include "stringio.h"
#include <stdlib.h>
#include <unistd.h>
#define ZENITY

#ifdef KDIALOG
int osio_save_file(char *path, char *filter) {
    path[0] = 0;
    char *cmd = "/bin/kdialog --getsavefilename . ";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(filter) * (strlen(filter)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), filter);
	FILE * fp = popen(fullcmd, "r");
	if (!fp) {
		printf("Failed to run command\n");
		return -1;
	}
    char *in = malloc(sizeof(path) * (strlen(path)+1));
	while (fgets(in, sizeof(in), fp) != NULL) {
        strcat(path, in);
    };
    path[strlen(path)-1] = 0;
    free(in);
    printf("%s\n", path);
    free(fullcmd);
	pclose(fp);
    return 0;
}

int osio_open_file(char *path, char *filter) {
    path[0] = 0;
    char *cmd = "/bin/kdialog --getopenfilename . ";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(filter) * (strlen(filter)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), filter);
	FILE * fp = popen(fullcmd, "r");
	if (!fp) {
		printf("Failed to run command\n");
		return -1;
	}
    char *in = malloc(sizeof(path) * (strlen(path)+1));
	while (fgets(in, sizeof(in), fp) != NULL) {
        strcat(path, in);
    };
    path[strlen(path)-1] = 0;
    free(in);
    free(fullcmd);
	pclose(fp);
    return 0;
}

int osio_print_error(char *msg) {
    char *cmd = "/bin/kdialog --error ";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(msg) * (strlen(msg)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), msg);
    FILE * fp = popen(fullcmd, "r");
    free(fullcmd);
    if (!fp) {
        printf("Failed to run command\n");
        return -1;
    }
    pclose(fp);
    return 0;
}

#endif

#ifdef ZENITY
int osio_save_file(char *path, char *filter) {
    path[0] = 0;
    char *cmd = "/bin/zenity --file-selection --save . ";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(filter) * (strlen(filter)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), filter);
	FILE * fp = popen(fullcmd, "r");
	if (!fp) {
		printf("Failed to run command\n");
		return -1;
	}
    char *in = malloc(sizeof(path) * (strlen(path)+1));
	while (fgets(in, sizeof(in), fp) != NULL) {
        strcat(path, in);
    };
    path[strlen(path)-1] = 0;
    free(in);
    printf("%s\n", path);
    free(fullcmd);
	pclose(fp);
    return 0;
}

int osio_open_file(char *path, char *filter) {
    path[0] = 0;
    char *cmd = "/bin/zenity --file-selection . ";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(filter) * (strlen(filter)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), filter);
	FILE * fp = popen(fullcmd, "r");
	if (!fp) {
		printf("Failed to run command\n");
		return -1;
	}
    char *in = malloc(sizeof(path) * (strlen(path)+1));
	while (fgets(in, sizeof(in), fp) != NULL) {
        strcat(path, in);
    };
    path[strlen(path)-1] = 0;
    free(in);
    free(fullcmd);
	pclose(fp);
    return 0;
}

int osio_print_error(char *msg) {
    char *cmd = "/bin/zenity --error ";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(msg) * (strlen(msg)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), msg);
    FILE * fp = popen(fullcmd, "r");
    free(fullcmd);
    if (!fp) {
        printf("Failed to run command\n");
        return -1;
    }
    pclose(fp);
    return 0;
}

#endif

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
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        #ifdef DEBUG
        printf("Current working directory: %s\n", cwd);
        #endif
    } else {
        perror("getcwd failed");
    }
    return 0;
}

int absolute_path_to_relative(char *path) {
    char cwd[200];
    char *str = malloc(sizeof(path) * (strlen(path)+1));
    strcpy(str, path);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        char *strcp = str;
        for (char *rpath = cwd;*rpath && *strcp; rpath++, strcp++);
        strcpy(path, ++strcp);
        free(str);
    } else {
        printf("Getcwd error!");
        return -1;
    }
    return 0;
}