#include "../io/input.h"
#include "signals.h"
struct Window;

/**
 * @file scripts.h
 * @brief Header file defining macros, structures, and functions for script management in a game engine.
 *
 * This file contains macros for handling script parameters, memory allocation for node attributes,
 * and script registration. It also defines structures and types for behavior attributes, scripts,
 * and behavior scripts enumeration.
 */

#ifndef SCRIPTS_H
#define SCRIPTS_H

/**
 * @brief Macro to define the parameters for a node function.
 */
#define NODE_FUNC_PARAMS struct Node *this, va_list args

/**
 * @brief Macro to define the return type for a node function.
 */
#define NODE_FUNC_RETURN void

/**
 * @brief Macro to retrieve the delta parameter from the variable argument list.
 */
#define GET_READY_PARAMETERS() float delta = (float) GET_PARAMETER(double); (void) delta

/**
 * @brief Macro to retrieve the signal parameter from the variable argument list.
 */
#define GET_SIGNAL() Signal signal = (Signal) GET_PARAMETER(int); (void) signal

/**
 * @brief Macro to retrieve a parameter of a specified type from the variable argument list.
 * @param type The type of the parameter to retrieve.
 */
#define GET_PARAMETER(type) va_arg(args, type)

/**
 * @brief Macro to retrieve parameters for an area signal from the variable argument list.
 */
#define GET_AREA_SIGNAL_PARAMETERS() int sigId = GET_PARAMETER(int); Node *other = (Node *) GET_PARAMETER(Node *); float distance = (float) GET_PARAMETER(double); float *impactPoint = (float *) GET_PARAMETER(float *); int offset = GET_PARAMETER(int); (void) sigId; (void) other; (void) distance; (void) impactPoint; (void) offset;

/**
 * @brief Macro to allocate memory for node attributes if not already allocated.
 * @param node The node for which to allocate attributes.
 * @param x The number of attributes to allocate.
 */
#define MALLOC_ATTRIBUTE(node, x) if (!node->attribute) { \
                                    node->attribute = malloc(sizeof(BehaviorAttribute) * x); \
                                    POINTER_CHECK(node->attribute); \
                                    node->attributes_count = x; \
                                    for (int i = 0; i < x; i++) memset(&node->attribute[i], 0, sizeof(union BehaviorAttribute)); \
                                }

/**
 * @brief Macro to set the number of attributes for the current node.
 * @param x The number of attributes to set.
 */
#define SET_ATTRIBUTES_COUNT(x) MALLOC_ATTRIBUTE(this, x)

/**
 * @brief Macro to define a new script function.
 * @param script_name The name of the script function.
 */
#define NEW_SCRIPT(script_name) NODE_FUNC_RETURN script_name(NODE_FUNC_PARAMS) {

/**
 * @brief Macro to end a script function and register it in the game engine.
 * @param script_name The name of the script function.
 */
#define END_SCRIPT(script_name) }; \
static __attribute__((constructor)) void __anon_ctor_##script_name(void) {\
    Game.scripts = realloc(Game.scripts, sizeof(Script) * (__scriptIndex__ + 1));\
    Game.scripts[__scriptIndex__].name = #script_name;\
    Game.scripts[__scriptIndex__++].script = script_name;\
    PRINT_INFO("Script %s loaded in %d!\n", #script_name, __scriptIndex__);\
}

/**
 * @union BehaviorAttribute
 * @brief Union representing different types of behavior attributes.
 */
typedef union BehaviorAttribute {
    int i;          /**< Integer attribute */
    float f;        /**< Float attribute */
    bool b;         /**< Boolean attribute */
    void *p;        /**< Pointer attribute */
    struct Node *node; /**< Node pointer attribute */
} BehaviorAttribute;

/**
 * @typedef ScriptFunc
 * @brief Function pointer type for script functions.
 */
typedef NODE_FUNC_RETURN (*ScriptFunc)(NODE_FUNC_PARAMS);

/**
 * @struct Script
 * @brief Structure representing a script with a function and a name.
 */
typedef struct Script {
    ScriptFunc script; /**< Function pointer to the script function */
    char * name;       /**< Name of the script */
} Script;

/**
 * @enum BehaviorScripts
 * @brief Enumeration of different behavior script types.
 */
typedef enum BehaviorScripts {
    BEHAVIOR_SCRIPT_READY,  /**< Ready script */
    BEHAVIOR_SCRIPT_UPDATE, /**< Update script */
    BEHAVIOR_SCRIPT_SIGNAL, /**< Signal script */
    BEHAVIOR_SCRIPT_COUNT,  /**< Total count of behavior scripts */
} BehaviorScripts;

/**
 * @typedef Behavior
 * @brief Array of script functions for different behavior scripts.
 */
typedef ScriptFunc Behavior[BEHAVIOR_SCRIPT_COUNT];

/**
 * @var __scriptIndex__
 * @brief Global index for tracking the number of registered scripts.
 */
extern int __scriptIndex__;

#endif // SCRIPTS_H