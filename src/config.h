#ifndef CONFIG_H
#define CONFIG_H

//#define DEBUG

enum ReturnSymbol {
    CLOSE_PROGRAM = -1,
    NO_ERROR,
};

#ifdef DEBUG
#include <signal.h>
#define BREAKPOINT printf("Breakpoint in %s at line %d\n", __FILE__, __LINE__), raise(SIGTRAP)
#define POINTER_CHECK(x) if (!x) printf("Pointer Exception in %s at line %d\n", __FILE__, __LINE__), exit(EXIT_FAILURE)
#else
#define BREAKPOINT
#define POINTER_CHECK(x)
#endif

#endif