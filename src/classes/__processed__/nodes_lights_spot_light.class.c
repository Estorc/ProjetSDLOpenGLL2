#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
void __class_method_spotlight_constructor(unsigned type, ...) {
unsigned __type__ = 14;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct SpotLight * spotLight = va_arg(args, struct SpotLight *);
va_end(args);
(void)this;
    this->object = spotLight;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_spotlight_cast(unsigned type, ...) {
unsigned __type__ = 14;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_spotlight_load(unsigned type, ...) {
unsigned __type__ = 14;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    SpotLight *spotLight;
    spotLight = malloc(sizeof(SpotLight));
    POINTER_CHECK(spotLight);

    if (file) {
        fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", 
            &spotLight->ambient[0], &spotLight->ambient[1], &spotLight->ambient[2], 
            &spotLight->diffuse[0], &spotLight->diffuse[1], &spotLight->diffuse[2], 
            &spotLight->specular[0], &spotLight->specular[1], &spotLight->specular[2],
            &spotLight->constant,
            &spotLight->linear,
            &spotLight->quadratic,
            &spotLight->cutOff,
            &spotLight->outerCutOff
            );
    } else {
        glm_vec3_zero(spotLight->ambient);
        glm_vec3_copy(GLM_VEC3_ONE, spotLight->diffuse);
        glm_vec3_copy(GLM_VEC3_ONE, spotLight->specular);
        spotLight->constant = 1.0f;
        spotLight->linear = 0.09f;
        spotLight->quadratic = 0.032f;
        spotLight->cutOff = 0.01f;
        spotLight->outerCutOff = 50.0f;
    }

    buffers.lightingBuffer.length++;
    METHOD_TYPE(this, CLASS_TYPE_SPOTLIGHT, constructor, spotLight);
    this->flags |= NODE_EDITOR_FLAG;
}

void __class_method_spotlight_save(unsigned type, ...) {
unsigned __type__ = 14;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    SpotLight *spotLight = (SpotLight*) this->object;
    fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
        spotLight->ambient[0], spotLight->ambient[1], spotLight->ambient[2], 
        spotLight->diffuse[0], spotLight->diffuse[1], spotLight->diffuse[2], 
        spotLight->specular[0], spotLight->specular[1], spotLight->specular[2],
        spotLight->constant,
        spotLight->linear,
        spotLight->quadratic,
        spotLight->cutOff,
        spotLight->outerCutOff
    );
}

