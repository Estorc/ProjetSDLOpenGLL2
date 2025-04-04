#include <raptiquax.h>
#include <classes/classes.h>

void call_method_0(void (*func)(void *, va_list), ...) {
    if (!func) return;
    va_list args;
    va_start(args, func);
    func(NULL, args);
    va_end(args);
}