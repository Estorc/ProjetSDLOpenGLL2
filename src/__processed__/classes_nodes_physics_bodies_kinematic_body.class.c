#include <stdarg.h>
#include "../types.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../io/scene_loader.h"
#include "../render/lighting.h"
#include "../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_KINEMATICBODY;
#line 13 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct KinematicBody * kinematicBody = va_arg(args, struct KinematicBody *);(void)this;
        this->object = kinematicBody;
        this->type = __type__;
        SUPER(initialize_node);
}

#line 19 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_get_length(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);u8 * length = va_arg(args, u8 *);(void)this;
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        *length = kinematicBody->length;
}

#line 24 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_get_collisions_shapes(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node *** shapes = va_arg(args, Node ***);u8 * length = va_arg(args, u8 *);(void)this;
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        *length = kinematicBody->length;
        *shapes = kinematicBody->collisionsShapes;
}

#line 30 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * pos = va_arg(args, vec3 *);vec3 * rot = va_arg(args, vec3 *);vec3 * scale = va_arg(args, vec3 *);(void)this;
        KinematicBody *kinematicBody = (KinematicBody *) this->object;

        glm_vec3_add(this->pos, kinematicBody->velocity, this->pos);
        this::update_global_position(pos, rot, scale);


        for (int i = 0; i < kinematicBody->length; i++) {
            (kinematicBody->collisionsShapes[i])::update_global_position(*pos, *rot, *scale);
            glm_vec3_copy(this->globalPos, *pos);
            glm_vec3_copy(this->globalRot, *rot);
            glm_vec3_copy(this->globalScale, *scale);
            check_collisions(kinematicBody->collisionsShapes[i]);
        }
        memcpy(&buffers.collisionBuffer.collisionsShapes[buffers.collisionBuffer.index], kinematicBody->collisionsShapes, kinematicBody->length * sizeof(kinematicBody->collisionsShapes[0]));
        buffers.collisionBuffer.index += kinematicBody->length;
}

#line 48 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Camera ** c = va_arg(args, Camera **);Script * scripts = va_arg(args, Script *);Node * editor = va_arg(args, Node *);(void)this;
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
        this->type = __type__;
        this::constructor(kinematicBody);

        kinematicBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(kinematicBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            kinematicBody->collisionsShapes[kinematicBody->length++] = child;
            child->parent = this;
        }
}

#line 75 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
        KinematicBody *kinematicBody = (KinematicBody*) this->object;
        u8 collisionsLength = kinematicBody->length;
        fprintf(file, "(%g,%g,%g,%d)",
        kinematicBody->velocity[0], kinematicBody->velocity[1], kinematicBody->velocity[2], 
        collisionsLength);
}


#line 85 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_apply_impulse(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float * impulse = va_arg(args, float *);float * torque = va_arg(args, float *);float * correction = va_arg(args, float *);(void)this;
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

    /**
     * @brief Get the velocity norm of a node.
     * 
     * @return The velocity norm of the node.
     */

float call_method_3(void (*func)(void *, va_list), ...) {
    va_list args;
    va_start(args, func);
    float value;
    func(&value, args);
    va_end(args);
    return value;
}
#line 105 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_get_velocity_norm(void * __retValueVP__, va_list args) {float * __retValueP__ = (float *) __retValueVP__;Node * this = va_arg(args, Node *);(void)this;
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        *__retValueP__ = glm_vec3_norm(kinematicBody->velocity);return;
}

    /**
     * @brief Get the velocity of a node.
     * 
     * @param velocity Output vector to store the velocity.
     */

#line 116 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_get_velocity(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * velocity = va_arg(args, vec3 *);(void)this;
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        glm_vec3_copy(kinematicBody->velocity, *velocity);
}

    /**
     * @brief Get the mass of a node.
     * 
     * @param mass Output pointer to store the mass.
     */

#line 127 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_get_mass(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float *  mass = va_arg(args, float * );(void)this;
        (*mass) = 100.0;
}

    /**
     * @brief Get the center of mass of a node.
     * 
     * @param com Output vector to store the center of mass.
     */

#line 137 "src/classes/nodes/physics/bodies/kinematic_body.class.c"
void __class_method_kinematicbody_get_center_of_mass(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * com = va_arg(args, vec3 *);(void)this;
        RigidBody *rigidBody = (RigidBody *) this->object;
        glm_vec3_copy(rigidBody->centerOfMass, *com);
}
