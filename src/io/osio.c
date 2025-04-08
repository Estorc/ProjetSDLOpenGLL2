#include <stdio.h>
#include <io/stringio.h>
#include <term/term.h>
#include <stdlib.h>
#include <unistd.h>
#include <io/osio.h>


#ifdef _WIN32
int osio_open_file(char *path, char *relativePath, char *filter) {
    OPENFILENAME ofn;       // Structure de sélection de fichier
    char szFile[MAX_PATH] = {0};  // Stocke le chemin du fichier sélectionné

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;  // Pas de fenêtre parent
    
    char * converted_filter = malloc(sizeof(filter) * (strlen(filter) + 1));
    strcpy(converted_filter, filter+1);

    printf("%s\n", converted_filter);
    char * parenthesis_pos = NULL;
    do {
        parenthesis_pos = strchr(converted_filter, '"');
        if (!parenthesis_pos)
            parenthesis_pos = strchr(converted_filter, ')');
        if (!parenthesis_pos)
            parenthesis_pos = strchr(converted_filter, '(');
        if (parenthesis_pos)
            *parenthesis_pos = '\0';
    } while(parenthesis_pos);

    printf("%s\n", converted_filter);
    
    ofn.lpstrFilter = converted_filter; // Filtre de fichier
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        path[0] = 0;
        strcat(path, szFile);
        free(converted_filter);
        update_cwd();
        return 0;
    } else {
        free(converted_filter);
        printf("Sélection annulée.\n");
        return -1;
    }
}


int osio_save_file(char *path, char *relativePath, char *filter) {
    OPENFILENAME ofn;       // Structure de sélection de fichier
    char szFile[MAX_PATH] = {0};  // Stocke le chemin du fichier sélectionné

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;  // Pas de fenêtre parent
    
    char * converted_filter = malloc(sizeof(filter) * (strlen(filter) + 1));
    strcpy(converted_filter, filter+1);

    printf("%s\n", converted_filter);
    char * parenthesis_pos = NULL;
    do {
        parenthesis_pos = strchr(converted_filter, '"');
        if (!parenthesis_pos)
            parenthesis_pos = strchr(converted_filter, ')');
        if (!parenthesis_pos)
            parenthesis_pos = strchr(converted_filter, '(');
        if (parenthesis_pos)
            *parenthesis_pos = '\0';
    } while(parenthesis_pos);

    printf("%s\n", converted_filter);
    
    ofn.lpstrFilter = converted_filter; // Filtre de fichier
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = 0;

    if (GetSaveFileName(&ofn)) {
        path[0] = 0;
        strcat(path, szFile);
        free(converted_filter);
        update_cwd();
        return 0;
    } else {
        free(converted_filter);
        printf("Sélection annulée.\n");
        return -1;
    }
}
#else
#define ZENITY
#endif

#ifdef KDIALOG
int osio_save_file(char *path, char *relativePath, char *filter) {
    path[0] = 0;
    char *cmd = "/bin/kdialog --getsavefilename $PWD/";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(relativePath) * (strlen(relativePath)+1) + sizeof(filter) * (strlen(filter)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), relativePath);
    fullcmd = strcat(fullcmd, " ");
    fullcmd = strcat(fullcmd, filter);
	FILE * fp = popen(fullcmd, "r");
	if (!fp) {
		PRINT_ERROR("Failed to run command\n");
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

int osio_open_file(char *path, char *relativePath, char *filter) {
    path[0] = 0;
    char *cmd = "/bin/kdialog --getopenfilename $PWD/";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(relativePath) * (strlen(relativePath)+1) + sizeof(filter) * (strlen(filter)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), relativePath);
    fullcmd = strcat(fullcmd, " ");
    fullcmd = strcat(fullcmd, filter);
	FILE * fp = popen(fullcmd, "r");
	if (!fp) {
		PRINT_ERROR("Failed to run command\n");
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
        PRINT_ERROR("Failed to run command\n");
        return -1;
    }
    pclose(fp);
    return 0;
}

#endif

#ifdef ZENITY
int osio_save_file(char *path, char *relativePath, char *filter) {
    path[0] = 0;
    char *cmd = "/bin/zenity --file-selection --save /home/estorc/ProjetSDLOpenGLL2 ";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(filter) * (strlen(filter)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), filter);
	FILE * fp = popen(fullcmd, "r");
	if (!fp) {
		PRINT_ERROR("Failed to run command\n");
		return -1;
	}
    char *in = malloc(sizeof(path) * (strlen(path)+1));
	while (fgets(in, sizeof(in), fp) != NULL) {
        strcat(path, in);
    };
    path[strlen(path)-1] = 0;
    free(in);
    PRINT_ERROR("%s\n", path);
    free(fullcmd);
	pclose(fp);
    return 0;
}

int osio_open_file(char *path, char *relativePath, char *filter) {
    path[0] = 0;
    char *cmd = "/bin/zenity --file-selection /home/estorc/ProjetSDLOpenGLL2 ";
    char *fullcmd = malloc(sizeof(cmd) * (strlen(cmd)+1) + sizeof(filter) * (strlen(filter)+1));
    fullcmd = strcat(strcpy(fullcmd, cmd), filter);
	FILE * fp = popen(fullcmd, "r");
	if (!fp) {
		PRINT_ERROR("Failed to run command\n");
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
        PRINT_ERROR("Failed to run command\n");
        return -1;
    }
    pclose(fp);
    return 0;
}

#endif


/*int osio_find(dirent_t ***namelist, const char * path, int (*filter)(const dirent_t *entry)) {
    int n = scandir(path, namelist, filter, alphasort);
    if (n < 0) {
        PRINT_ERROR("scandir failed\n");
        return n;
    }
    return n;
}*/

int update_cwd() {
    char exe_path[1024]; // Buffer to store the executable path.

    // Read the symbolic link /proc/self/exe to get the full path of the executable.
    #ifdef __linux__
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    #elif _WIN32
    DWORD len = GetModuleFileName(NULL, exe_path, MAX_PATH);
    if (len == 0) {
        PRINT_ERROR("GetModuleFileName failed\n");
        return -1;
    }
    #endif
    if (len == -1) {
        perror("readlink failed");
        return -1;
    }

    exe_path[len] = '\0'; // Null-terminate the path.
    // Extract the directory from the full path.
    #ifdef __linux__
    char *last_slash = strrchr(exe_path, '/');
    #elif _WIN32
    char *last_slash = strrchr(exe_path, '\\');
    #endif
    if (last_slash != NULL) {
        *last_slash = '\0'; // Remove the executable name to get the directory.
    } else {
        PRINT_ERROR("Error: Unable to determine directory of executable\n");
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
        PRINT_INFO("Current working directory: %s\n", cwd);
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
        PRINT_INFO("%s\n", cwd);
        char *strcp = str;
        for (char *rpath = cwd;*rpath && *strcp; rpath++, strcp++);
        strcpy(path, ++strcp);
        free(str);
    } else {
        PRINT_ERROR("Getcwd error!");
        return -1;
    }
    return 0;
}