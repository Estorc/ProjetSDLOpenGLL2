#include "../io/input.h"
#include "signals.h"
struct Window;

#define NODE_FUNC_PARAMS struct Node *this, va_list args
#define NODE_FUNC_RETURN void
#define GET_READY_PARAMETERS() float delta = (float) GET_PARAMETER(double); (void) delta
#define GET_SIGNAL() Signal signal = (Signal) GET_PARAMETER(int); (void) signal
#define GET_PARAMETER(type) va_arg(args, type)
#define GET_AREA_SIGNAL_PARAMETERS() int sigId = GET_PARAMETER(int); Node *other = (Node *) GET_PARAMETER(Node *); float distance = (float) GET_PARAMETER(double); float *impactPoint = (float *) GET_PARAMETER(float *); (void) sigId; (void) other; (void) distance; (void) impactPoint
#define MALLOC_ATTRIBUTE(node, x) if (!node->attribute) { \
                                    node->attribute = malloc(sizeof(BehaviorAttribute) * x); \
                                    POINTER_CHECK(node->attribute); \
                                    node->attributes_count = x; \
                                    for (int i = 0; i < x; i++) memset(&node->attribute[i], 0, sizeof(union BehaviorAttribute)); \
                                }
#define SET_ATTRIBUTES_COUNT(x) MALLOC_ATTRIBUTE(this, x)

#define NEW_SCRIPT(script_name) NODE_FUNC_RETURN script_name(NODE_FUNC_PARAMS) {
#define END_SCRIPT(script_name) }; \
static __attribute__((constructor)) void __anon_ctor_##script_name(void) {\
    mainNodeTree.scripts = realloc(mainNodeTree.scripts, sizeof(Script) * (__scriptIndex__ + 1));\
    mainNodeTree.scripts[__scriptIndex__].name = #script_name;\
    mainNodeTree.scripts[__scriptIndex__++].script = script_name;\
    PRINT_INFO("Script %s loaded in %d!\n", #script_name, __scriptIndex__);\
}

#ifndef SCRIPTS_H
#define SCRIPTS_H

typedef union BehaviorAttribute {
    int i;
    float f;
    bool b;
    void *p;
    struct Node *node;
} BehaviorAttribute;

typedef NODE_FUNC_RETURN (*ScriptFunc)(NODE_FUNC_PARAMS);

typedef struct Script {
    ScriptFunc script;
    char * name;
} Script;

typedef enum BehaviorScripts {
    BEHAVIOR_SCRIPT_READY,
    BEHAVIOR_SCRIPT_UPDATE,
    BEHAVIOR_SCRIPT_SIGNAL,
    BEHAVIOR_SCRIPT_COUNT,
} BehaviorScripts;

typedef ScriptFunc Behavior[BEHAVIOR_SCRIPT_COUNT];
extern int __scriptIndex__;

#endif