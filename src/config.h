#ifndef CONFIG_H
#define CONFIG_H

#ifndef _WIN32
enum ReturnSymbol {
    CLOSE_PROGRAM = -1,
    NO_ERROR,
};
#endif


#include "term/term.h"
#define UNUSED(x) (void)x;
#ifdef DEBUG
#include <signal.h>
#define BREAKPOINT printf("Breakpoint in %s at line %d\n", __FILE__, __LINE__), raise(SIGTRAP)
#define POINTER_CHECK(x) if (!x) printf("Pointer Exception in %s at line %d\n", __FILE__, __LINE__), exit(EXIT_FAILURE)
#else
#define BREAKPOINT
#define POINTER_CHECK(x)
#endif 
#endif