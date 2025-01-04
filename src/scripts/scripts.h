#include "../io/input.h"
struct Window;
#define NODE_FUNC_PARAMS struct Node *node, Input *input, struct Window *window, float delta
#define NODE_FUNC_RETURN void
#define PARAMS_COUNT(x) if (!node->params) { \
		                    node->params = malloc(sizeof(BehaviorParameter) * x); \
		                    POINTER_CHECK(node->params); \
                            node->params_count = x; \
                            for (int i = 0; i < x; i++) memset(&node->params[i], 0, sizeof(union BehaviorParameter)); \
	                    }

#define NEW_SCRIPT(script_name) NODE_FUNC_RETURN script_name(NODE_FUNC_PARAMS) {
#define END_SCRIPT(script_name) }; mainNodeTree.scripts[mainNodeTree.scriptIndex].name = #script_name, mainNodeTree.scripts[mainNodeTree.scriptIndex++].script = script_name;

#ifndef SCRIPTS_H
#define SCRIPTS_H

typedef union BehaviorParameter {
    int i;
    float f;
    bool b;
    struct Node *node;
} BehaviorParameter;

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