#include <stdio.h>
#include <unistd.h>
#ifdef __windows__
#include <windows.h>
#endif

void fast_vec3_print(const float *v, const char *name) {
    char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "%s (%.3f, %.3f, %.3f)\n", name, v[0], v[1], v[2]);
    
    #ifdef __windows__
    DWORD written;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, len, &written, NULL);
    #else
    write(STDOUT_FILENO, buffer, len);
    #endif
}