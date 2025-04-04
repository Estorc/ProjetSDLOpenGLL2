#pragma once

#define VOID_FILE "NUL"
#include <winsock2.h>
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

#include <io/dirent.c>