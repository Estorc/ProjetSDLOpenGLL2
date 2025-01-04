#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_POINTLIGHT;


void __class_method_pointlight_constructor(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
struct PointLight * pointLight = va_arg(args, struct PointLight *);
(void)this;
    this->object = pointLight;
    this->type = __type__;
    SUPER(initialize_node);
    SUPER(init_light);
}


void __class_method_pointlight_cast(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
(void)this;
    IGNORE(data);
}


void __class_method_pointlight_load(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
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
    METHOD_TYPE(this, __type__, constructor, pointLight);
    this->flags |= NODE_EDITOR_FLAG;
}


void __class_method_pointlight_save(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
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


    

