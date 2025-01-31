#include "../io/input.h"
struct Window;
#define NODE_FUNC_PARAMS struct Node *this, va_list args
#define NODE_FUNC_RETURN void
#define GET_READY_PARAMETERS() float delta = (float) GET_PARAMETER(double); (void) delta
#define GET_PARAMETER(type) va_arg(args, type)
#define MALLOC_ATTRIBUTE(node, x) if (!node->attribute) { \
                                    node->attribute = malloc(sizeof(BehaviorAttribute) * x); \
                                    POINTER_CHECK(node->attribute); \
                                    node->attributes_count = x; \
                                    for (int i = 0; i < x; i++) memset(&node->attribute[i], 0, sizeof(union BehaviorAttribute)); \
                                }
#define SET_ATTRIBUTES_COUNT(x) MALLOC_ATTRIBUTE(this, x)

#define NEW_SCRIPT(script_name) NODE_FUNC_RETURN script_name(NODE_FUNC_PARAMS) {
#define END_SCRIPT(script_name) }; mainNodeTree.scripts[mainNodeTree.scriptIndex].name = #script_name, mainNodeTree.scripts[mainNodeTree.scriptIndex++].script = script_name;

#ifndef SCRIPTS_H
#define SCRIPTS_H

typedef union BehaviorAttribute {
    int i;
    float f;
    bool b;
    struct Node *node;
} BehaviorAttribute;

typedef struct Script {
    NODE_FUNC_RETURN (*script)(NODE_FUNC_PARAMS);
    char * name;
} Script;

typedef enum BehaviorScripts {
    BEHAVIOR_SCRIPT_READY,
    BEHAVIOR_SCRIPT_UPDATE,
    BEHAVIOR_SCRIPT_SIGNAL,
    BEHAVIOR_SCRIPT_COUNT,
} BehaviorScripts;

typedef NODE_FUNC_RETURN (*Behavior[BEHAVIOR_SCRIPT_COUNT])(NODE_FUNC_PARAMS);

#endif