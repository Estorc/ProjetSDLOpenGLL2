#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
#include "../../io/scene_loader.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
void __class_method_kinematicbody_constructor(unsigned type, ...) {
unsigned __type__ = 2;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct KinematicBody * kinematicBody = va_arg(args, struct KinematicBody *);
va_end(args);
(void)this;
    this->object = kinematicBody;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_kinematicbody_cast(unsigned type, ...) {
unsigned __type__ = 2;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_kinematicbody_load(unsigned type, ...) {
unsigned __type__ = 2;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Camera ** c = va_arg(args, Camera **);
Script * scripts = va_arg(args, Script *);
Node * editor = va_arg(args, Node *);
va_end(args);
(void)this;
    KinematicBody *kinematicBody;
    kinematicBody = malloc(sizeof(KinematicBody));
    kinematicBody->length = 0;
    int children_count = 0;
    POINTER_CHECK(kinematicBody);
    if (file) {
        fscanf(file,"(%g,%g,%g,%d)\n", 
            &kinematicBody->velocity[0], &kinematicBody->velocity[1], &kinematicBody->velocity[2], 
            &children_count);
    } else {
        glm_vec3_zero(kinematicBody->velocity);
    }
    METHOD_TYPE(this, CLASS_TYPE_KINEMATICBODY, constructor, kinematicBody);

    kinematicBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
    buffers.collisionBuffer.length += children_count;
    POINTER_CHECK(kinematicBody->collisionsShapes);
    
    for (int i = 0; i < children_count; i++) {
        Node *child = load_node(file, c, scripts, editor);
        kinematicBody->collisionsShapes[kinematicBody->length++] = child;
        child->parent = this;
    }
}

void __class_method_kinematicbody_save(unsigned type, ...) {
unsigned __type__ = 2;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    KinematicBody *kinematicBody = (KinematicBody*) this->object;
    u8 collisionsLength = kinematicBody->length;
    fprintf(file, "(%g,%g,%g,%d)",
    kinematicBody->velocity[0], kinematicBody->velocity[1], kinematicBody->velocity[2], 
    collisionsLength);
}

void __class_method_kinematicbody_apply_impulse(unsigned type, ...) {
unsigned __type__ = 2;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
float * impulse = va_arg(args, float *);
float * torque = va_arg(args, float *);
float * correction = va_arg(args, float *);
va_end(args);
(void)this;
    IGNORE(torque);
    KinematicBody *kinematicBody = (KinematicBody *) this->object;

    // Change velocity
    glm_vec3_add(kinematicBody->velocity, impulse, kinematicBody->velocity);

    // Move shape A out of shape B
    glm_vec3_sub(this->pos, correction, this->pos);
    for (int i = 0; i < kinematicBody->length; i++) {
        glm_vec3_sub(kinematicBody->collisionsShapes[i]->globalPos, correction, kinematicBody->collisionsShapes[i]->globalPos);
    }
}

