#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int osio_save_file(char *path, char *filter) {
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

int absolute_path_to_relative(char *path) {
    char cwd[100];
    char *str = malloc(sizeof(path) * (strlen(path)+1));
    strcpy(str, path);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
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