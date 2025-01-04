#include <stdarg.h>
#include <stdlib.h>
#include "../types.h"
#include "classes.h"

int call_and_get(void (*func)(void *, va_list), ...) {
    va_list args;
    va_start(args, func);
    int value;
    func(&value, args);
    va_end(args);
    return value;
}

void call_method(void (*func)(void *, va_list), ...) {
    va_list args;
    va_start(args, func);
    func(NULL, args);
    va_end(args);
}