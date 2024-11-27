#include "../io/input.h"
struct Window;
#define NODE_FUNC_PARAMS struct Node *node, Input *input, struct Window *window, float delta
#define NODE_FUNC_RETURN void
#define PARAMS_COUNT(x) if (!node->params) { \
		                    node->params = malloc(sizeof(ScriptParameter) * x); \
		                    POINTER_CHECK(node->params); \
                            node->params_count = x; \
                            for (int i = 0; i < x; i++) memset(&node->params[i], 0, sizeof(union ScriptParameter)); \
	                    }

#define NEW_SCRIPT(script_name) NODE_FUNC_RETURN script_name(NODE_FUNC_PARAMS) {
#define END_SCRIPT(script_name) }; scripts[scriptIndex].name = #script_name, scripts[scriptIndex++].script = script_name;

#ifndef SCRIPTS_H
#define SCRIPTS_H

typedef union ScriptParameter {
    int i;
    float f;
    vec3 v3;
    struct Node *node;
} ScriptParameter;

typedef struct Script {
    NODE_FUNC_RETURN (*script)(NODE_FUNC_PARAMS);
    char * name;
} Script;

#endif