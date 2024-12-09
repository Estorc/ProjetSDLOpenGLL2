#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
void __class_method_directionallight_constructor(unsigned type, ...) {
unsigned __type__ = 12;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct DirectionalLight * directionalLight = va_arg(args, struct DirectionalLight *);
va_end(args);
(void)this;
    this->object = directionalLight;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_directionallight_cast(unsigned type, ...) {
unsigned __type__ = 12;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_directionallight_load(unsigned type, ...) {
unsigned __type__ = 12;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    DirectionalLight *directionalLight;
    directionalLight = malloc(sizeof(DirectionalLight));
    POINTER_CHECK(directionalLight);

    if (file) {
        fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", 
            &directionalLight->ambient[0], &directionalLight->ambient[1], &directionalLight->ambient[2], 
            &directionalLight->diffuse[0], &directionalLight->diffuse[1], &directionalLight->diffuse[2], 
            &directionalLight->specular[0], &directionalLight->specular[1], &directionalLight->specular[2],
            &directionalLight->constant,
            &directionalLight->linear,
            &directionalLight->quadratic
            );
    } else {
        glm_vec3_zero(directionalLight->ambient);
        glm_vec3_copy(GLM_VEC3_ONE, directionalLight->diffuse);
        glm_vec3_copy(GLM_VEC3_ONE, directionalLight->specular);
        directionalLight->constant = 1.0f;
        directionalLight->linear = 0.09f;
        directionalLight->quadratic = 0.032f;
    }

    buffers.lightingBuffer.length++;
    METHOD_TYPE(this, CLASS_TYPE_DIRECTIONALLIGHT, constructor, directionalLight);
    this->flags |= NODE_EDITOR_FLAG;
}

void __class_method_directionallight_save(unsigned type, ...) {
unsigned __type__ = 12;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    DirectionalLight *directionalLight = (DirectionalLight*) this->object;
    fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
        directionalLight->ambient[0], directionalLight->ambient[1], directionalLight->ambient[2], 
        directionalLight->diffuse[0], directionalLight->diffuse[1], directionalLight->diffuse[2], 
        directionalLight->specular[0], directionalLight->specular[1], directionalLight->specular[2],
        directionalLight->constant,
        directionalLight->linear,
        directionalLight->quadratic
    );
}

