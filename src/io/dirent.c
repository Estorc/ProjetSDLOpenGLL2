#include <stdio.h>
#include <term/term.h>
#include <stdlib.h>
#include <unistd.h>
#include <io/dirent.h>

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

int alphasort_portable(const dirent_t **a, const dirent_t **b) {
    return strcmp((*a)->d_name, (*b)->d_name);
}