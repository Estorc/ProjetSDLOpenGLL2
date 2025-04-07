#pragma once
#include <dirent.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>

#define DT_UNKNOWN  0
#define DT_REG      8
#define DT_DIR      4

struct dirent_win {
    char d_name[MAX_PATH];
    unsigned char d_type;
};
typedef struct dirent_win dirent_t;

int scandir_windows(const char *dirpath, dirent_t ***namelist,
                    int (*filter)(const dirent_t *),
                    int (*compar)(const dirent_t **, const dirent_t **));
int alphasort_portable(const dirent_t **a, const dirent_t **b);

#define scandir(dirpath, namelist, filter, compar) scandir_windows(dirpath, namelist, filter, compar)
#define alphasort alphasort_portable
#else
typedef struct dirent dirent_t;
#endif