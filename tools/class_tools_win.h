#pragma once

#define VOID_FILE "NUL"
#include <windows.h>
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)

ssize_t readlink_windows(const char *path, char *buf, size_t bufsize) {
    HANDLE hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
                               OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }

    DWORD size = GetFinalPathNameByHandleA(hFile, buf, (DWORD)bufsize, FILE_NAME_NORMALIZED);
    CloseHandle(hFile);

    if (size == 0 || size >= bufsize) {
        return -1; // Erreur ou buffer trop petit
    }

    return (ssize_t)size;
}

#define readlink(path, buf, bufsize) readlink_windows(path, buf, bufsize)

// Définition des types de fichiers (comme Linux)
#define DT_UNKNOWN  0
#define DT_REG      8
#define DT_DIR      4

struct dirent_win {
    char d_name[MAX_PATH];
    unsigned char d_type;  // Ajout du type de fichier
};

typedef struct dirent_win dirent_t;

int scandir_windows(const char *dirpath, dirent_t ***namelist,
                    int (*filter)(const dirent_t *),
                    int (*compar)(const dirent_t **, const dirent_t **)) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH];

    snprintf(searchPath, MAX_PATH, "%s\\*", dirpath);
    hFind = FindFirstFileA(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return -1;
    }

    dirent_t **list = NULL;
    int count = 0;

    do {
        dirent_t *entry = malloc(sizeof(dirent_t));
        if (!entry) {
            perror("malloc");
            break;
        }
        strncpy(entry->d_name, findFileData.cFileName, MAX_PATH);

        // Déterminer le type de fichier
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            entry->d_type = DT_DIR;
        } else {
            entry->d_type = DT_REG;
        }

        if (!filter || filter(entry)) {
            list = realloc(list, (count + 1) * sizeof(dirent_t *));
            if (!list) {
                perror("realloc");
                free(entry);
                break;
            }
            list[count++] = entry;
        } else {
            free(entry);
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (compar) {
        qsort(list, count, sizeof(dirent_t *),
              (int (*)(const void *, const void *))compar);
    }

    *namelist = list;
    return count;
}


#define scandir(dirpath, namelist, filter, compar) scandir_windows(dirpath, namelist, filter, compar)


int alphasort_portable(const dirent_t **a, const dirent_t **b) {
    return strcmp((*a)->d_name, (*b)->d_name);
}

#define alphasort alphasort_portable