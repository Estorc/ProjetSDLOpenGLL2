#pragma once
#include "import_class.h"
#include "../types.h"

typedef struct Object {
    void *object;
    u8 type;
} Object;

#define SUPER(method_name, ...) {\
    ClassType type = classManager.extends[__type__];\
    void (*method)(unsigned type, ...) = classManager.methodsCorrespondance.method_name[type];\
    if (method) method(type, this, ##__VA_ARGS__);\
    else {\
        while (!method) {\
            if (type == -1) {\
                fprintf(stderr, "Object doesn't have method: %s\n", "method_name");\
                break;\
            }\
            type = classManager.extends[type];\
            method = classManager.methodsCorrespondance.method_name[type];\
        }\
        method(type, this, ##__VA_ARGS__);\
    }\
}
#define METHOD_TYPE(obj, default_type, method_name, ...) {\
void (*method)(unsigned type, ...) = classManager.methodsCorrespondance.method_name[default_type];\
if (method) method(default_type, obj, ##__VA_ARGS__);\
else {\
    ClassType type = classManager.extends[default_type];\
    while (!method) {\
        if (type == -1) {\
            fprintf(stderr, "Object doesn't have method: %s\n", "method_name");\
            break;\
        }\
        method = classManager.methodsCorrespondance.method_name[type];\
        type = classManager.extends[type];\
    }\
    method(default_type, obj, ##__VA_ARGS__);\
}};
#define METHOD(obj, method_name, ...) METHOD_TYPE(obj, obj->type, method_name, ##__VA_ARGS__)