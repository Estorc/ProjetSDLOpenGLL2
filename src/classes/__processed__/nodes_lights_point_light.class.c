#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
void __class_method_pointlight_constructor(unsigned type, ...) {
unsigned __type__ = 13;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct PointLight * pointLight = va_arg(args, struct PointLight *);
va_end(args);
(void)this;
    this->object = pointLight;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_pointlight_cast(unsigned type, ...) {
unsigned __type__ = 13;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_pointlight_load(unsigned type, ...) {
unsigned __type__ = 13;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    PointLight *pointLight;
    pointLight = malloc(sizeof(PointLight));
    POINTER_CHECK(pointLight);

    if (file) {
        fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", 
            &pointLight->ambient[0], &pointLight->ambient[1], &pointLight->ambient[2], 
            &pointLight->diffuse[0], &pointLight->diffuse[1], &pointLight->diffuse[2], 
            &pointLight->specular[0], &pointLight->specular[1], &pointLight->specular[2],
            &pointLight->constant,
            &pointLight->linear,
            &pointLight->quadratic
            );
    } else {
        glm_vec3_zero(pointLight->ambient);
        glm_vec3_copy(GLM_VEC3_ONE, pointLight->diffuse);
        glm_vec3_copy(GLM_VEC3_ONE, pointLight->specular);
        pointLight->constant = 1.0f;
        pointLight->linear = 0.09f;
        pointLight->quadratic = 0.032f;
    }

    buffers.lightingBuffer.length++;
    METHOD_TYPE(this, CLASS_TYPE_POINTLIGHT, constructor, pointLight);
    this->flags |= NODE_EDITOR_FLAG;
}

void __class_method_pointlight_save(unsigned type, ...) {
unsigned __type__ = 13;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    PointLight *pointLight = (PointLight*) this->object;
    fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
        pointLight->ambient[0], pointLight->ambient[1], pointLight->ambient[2], 
        pointLight->diffuse[0], pointLight->diffuse[1], pointLight->diffuse[2], 
        pointLight->specular[0], pointLight->specular[1], pointLight->specular[2],
        pointLight->constant,
        pointLight->linear,
        pointLight->quadratic
    );
}

