#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../io/scene_loader.h"
#include "../render/lighting.h"
#include "../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_RIGIDBODY;
#line 13 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct RigidBody * rigidBody = va_arg(args, struct RigidBody *);(void)this;
        this->object = rigidBody;
        this->type = __type__;
        SUPER(initialize_node);
}

#line 19 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_get_collisions_shapes(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node **** shapes = va_arg(args, Node ****);u8 ** length = va_arg(args, u8 **);(void)this;
        RigidBody *rigidBody = (RigidBody *) this->object;
        *length = &rigidBody->length;
        *shapes = &rigidBody->collisionsShapes;
}

#line 25 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * pos = va_arg(args, vec3 *);vec3 * rot = va_arg(args, vec3 *);vec3 * scale = va_arg(args, vec3 *);double  delta = va_arg(args, double );(void)this;
        RigidBody *rigidBody = (RigidBody *) this->object;

        vec3 gravity;
        glm_vec3_scale(rigidBody->gravity, delta, gravity);
        glm_vec3_add(rigidBody->velocity, gravity, rigidBody->velocity);
        glm_vec3_scale(rigidBody->velocity, rigidBody->friction * (1.0-delta), rigidBody->velocity);
        glm_vec3_add(this->pos, rigidBody->velocity, this->pos);

        glm_vec3_scale(rigidBody->angularVelocity, rigidBody->friction * (1.0-delta), rigidBody->angularVelocity);
        glm_vec3_add(this->rot, rigidBody->angularVelocity, this->rot);

        this::update_global_position(pos, rot, scale);


        for (int i = 0; i < rigidBody->length; i++) {
            (rigidBody->collisionsShapes[i])::update_global_position(*pos, *rot, *scale);
            glm_vec3_copy(this->globalPos, *pos);
            glm_vec3_copy(this->globalRot, *rot);
            glm_vec3_copy(this->globalScale, *scale);
            check_collisions(rigidBody->collisionsShapes[i]);
        }
        memcpy(&buffers.collisionBuffer.collisionsShapes[buffers.collisionBuffer.index], rigidBody->collisionsShapes, rigidBody->length * sizeof(rigidBody->collisionsShapes[0]));
        buffers.collisionBuffer.index += rigidBody->length;
}

#line 51 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Camera ** c = va_arg(args, Camera **);Script * scripts = va_arg(args, Script *);Node * editor = va_arg(args, Node *);(void)this;
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
        this->type = __type__;
        this::constructor(rigidBody);

        rigidBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(rigidBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            rigidBody->collisionsShapes[rigidBody->length++] = child;
            child->parent = this;
        }
}

#line 88 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
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

#line 102 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_apply_impulse(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float * impulse = va_arg(args, float *);float * torque = va_arg(args, float *);float * correction = va_arg(args, float *);float * momentOfInertia = va_arg(args, float *);(void)this;
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

    /**
     * @brief Get the velocity norm of a node.
     * 
     * @return The velocity norm of the node.
     */

#line 125 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_get_velocity_norm(void * __retValueVP__, va_list args) {float * __retValueP__ = (float *) __retValueVP__;Node * this = va_arg(args, Node *);(void)this;
        RigidBody *rigidBody = (RigidBody *) this->object;
        *__retValueP__ = glm_vec3_norm(rigidBody->velocity);return;
}

    /**
     * @brief Get the velocity of a node.
     * 
     * @param velocity Output vector to store the velocity.
     */

#line 136 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_get_velocity(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * velocity = va_arg(args, vec3 *);(void)this;
        RigidBody *rigidBody = (RigidBody *) this->object;
        glm_vec3_copy(rigidBody->velocity, *velocity);
}

    /**
     * @brief Get the mass of a node.
     * 
     * @param mass Output pointer to store the mass.
     */

#line 147 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_get_mass(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float *  mass = va_arg(args, float * );(void)this;
        RigidBody *rigidBody = (RigidBody *) this->object;
        (*mass) = rigidBody->mass;
}

    /**
     * @brief Get the center of mass of a node.
     * 
     * @param com Output vector to store the center of mass.
     */

#line 158 "src/classes/nodes/physics/bodies/rigid_body.class.c"
void __class_method_rigidbody_get_center_of_mass(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * com = va_arg(args, vec3 *);(void)this;
        RigidBody *rigidBody = (RigidBody *) this->object;
        glm_vec3_copy(rigidBody->centerOfMass, *com);
}
    
