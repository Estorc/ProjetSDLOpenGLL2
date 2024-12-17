#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
#include "../../io/scene_loader.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_RIGIDBODY;


void __class_method_rigidbody_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct RigidBody * rigidBody = va_arg(args, struct RigidBody *);
va_end(args);
(void)this;
    this->object = rigidBody;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_rigidbody_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_rigidbody_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Camera ** c = va_arg(args, Camera **);
Script * scripts = va_arg(args, Script *);
Node * editor = va_arg(args, Node *);
va_end(args);
(void)this;
    RigidBody *rigidBody;
    rigidBody = malloc(sizeof(RigidBody));
    rigidBody->length = 0;
    int children_count = 0;
    POINTER_CHECK(rigidBody);
    if (file) {
        fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%d)\n", 
            &rigidBody->velocity[0], &rigidBody->velocity[1], &rigidBody->velocity[2], 
            &rigidBody->angularVelocity[0], &rigidBody->angularVelocity[1], &rigidBody->angularVelocity[2], 
            &rigidBody->gravity[0], &rigidBody->gravity[1], &rigidBody->gravity[2],
            &rigidBody->mass,
            &rigidBody->friction,
            &rigidBody->centerOfMass[0], &rigidBody->centerOfMass[1], &rigidBody->centerOfMass[2],
            &children_count);
    } else {
        glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->velocity);
        glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->angularVelocity);
        glm_vec3_copy((vec3) {0.0,-1.0,0.0}, rigidBody->gravity);
        rigidBody->mass = 100.0;
        rigidBody->friction = 0.98;
        glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->centerOfMass);
    }
    METHOD_TYPE(this, __type__, constructor, rigidBody);

    rigidBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
    buffers.collisionBuffer.length += children_count;
    POINTER_CHECK(rigidBody->collisionsShapes);
    
    for (int i = 0; i < children_count; i++) {
        Node *child = load_node(file, c, scripts, editor);
        rigidBody->collisionsShapes[rigidBody->length++] = child;
        child->parent = this;
    }
}


void __class_method_rigidbody_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    RigidBody *rigidBody = (RigidBody*) this->object;
    u8 collisionsLength = rigidBody->length;
    fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%d)",
    rigidBody->velocity[0], rigidBody->velocity[1], rigidBody->velocity[2], 
    rigidBody->angularVelocity[0], rigidBody->angularVelocity[1], rigidBody->angularVelocity[2], 
    rigidBody->gravity[0], rigidBody->gravity[1], rigidBody->gravity[2],
    rigidBody->mass,
    rigidBody->friction,
    rigidBody->centerOfMass[0], rigidBody->centerOfMass[1], rigidBody->centerOfMass[2],
    collisionsLength);
}


void __class_method_rigidbody_apply_impulse(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
float * impulse = va_arg(args, float *);
float * torque = va_arg(args, float *);
float * correction = va_arg(args, float *);
float * momentOfInertia = va_arg(args, float *);
va_end(args);
(void)this;
    RigidBody *rigidBody = (RigidBody *) this->object;

    // Change velocity
    glm_vec3_add(rigidBody->velocity, impulse, rigidBody->velocity);

    // Move shape A out of shape B
    glm_vec3_sub(this->pos, correction, this->pos);
    for (int i = 0; i < rigidBody->length; i++) {
        glm_vec3_sub(rigidBody->collisionsShapes[i]->globalPos, correction, rigidBody->collisionsShapes[i]->globalPos);
    }

    // Apply torque
    glm_vec3_scale(torque, *momentOfInertia*50.0f, torque);
    glm_vec3_add(rigidBody->angularVelocity, torque, rigidBody->angularVelocity);
}




    

