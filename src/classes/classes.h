#pragma once
#include "import_class.h"
#include "../raptiquax.h"

/**
 * @brief Defines the Object structure and macros for method invocation in a class hierarchy.
 * 
 * This file contains the definition of the Object structure and several macros to facilitate
 * method invocation in a class hierarchy. The macros handle method lookup and invocation,
 * including support for inheritance and method overriding.
 * @defgroup Classes Classes
 * @{
 */


/**
 * @struct Object
 * @brief A generic object structure.
 * 
 * This structure represents a generic object with a pointer to the actual object data
 * and a type identifier.
 */

typedef struct Object {
    void *object; /** < A pointer to the actual object data. */
    u8 type; /** < An unsigned 8-bit integer representing the type of the object. */
} Object;

/**
 * @def call_method_0(func, ...)
 * @brief Invokes a method on an object.
 * 
 * This function is used to invoke a method on an object. It takes a function pointer
 * to the method to invoke and a variable number of arguments to pass to the method.
 * 
 * @param func A function pointer to the method to invoke.
 * @param ... Additional arguments to pass to the method.
 * 
 * @note This function is used internally by the METHOD and SUPER macros.
 * 0 is the default call_method function references void return.
 * 
 * @see METHOD
 */

void call_method_0(void (*func)(void *, va_list), ...);

/**
 * @def SUPER(method_name, ...)
 * @brief Invokes a method from the superclass.
 * 
 * This macro is used to invoke a method from the superclass of the current object.
 * It performs a lookup in the class hierarchy to find the appropriate method and
 * invokes it with the provided arguments.
 * 
 * @param method_name The name of the method to invoke.
 * @param ... Additional arguments to pass to the method.
 */

#define SUPER(method_name, ...) call_method_0(classManager.methodsCorrespondance.method_name[classManager.extends[__type__]], this, ##__VA_ARGS__)

/**
 * @def METHOD_TYPE(obj, default_type, method_name, ...)
 * @brief Invokes a method with a specified default type.
 * 
 * This macro is used to invoke a method on an object with a specified default type.
 * It performs a lookup in the class hierarchy to find the appropriate method and
 * invokes it with the provided arguments.
 * 
 * @param default_type The default type to use for method lookup.
 * @param method_name The name of the method to invoke.
 * @param obj The object on which to invoke the method.
 * @param ... Additional arguments to pass to the method.
 */

#define METHOD_TYPE(default_type, method_name, obj, ...) classManager.methodsCorrespondance.method_name[default_type], obj, ##__VA_ARGS__

/**
 * @def METHOD(obj, method_name, ...)
 * @brief Invokes a method on an object.
 * 
 * This macro is used to invoke a method on an object. It uses the object's type
 * to perform a lookup in the class hierarchy and find the appropriate method,
 * then invokes it with the provided arguments.
 * 
 * @param method_name The name of the method to invoke.
 * @param obj The object on which to invoke the method.
 * @param ... Additional arguments to pass to the method.
 */

#define METHOD(method_name, obj, ...) classManager.methodsCorrespondance.method_name[obj->type], obj, ##__VA_ARGS__

/** @} */