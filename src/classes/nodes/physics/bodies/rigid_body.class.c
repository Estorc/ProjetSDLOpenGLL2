/**
 * @file
 * @brief This source file defines the implementation of rigid body physics for the project.
 *
 * This source file contains the implementation of the RigidBody class, which is responsible for
 * handling the physics calculations and properties of rigid bodies in the simulation. It includes
 * functions for updating the state of the rigid bodies, applying forces, and handling collisions.
 * The primary use case of this code is to provide realistic physics behavior for objects in the
 * simulation.
 *
 * @author Loup Picault
 * @date 2023-10-30
 */

#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class RigidBody : public Body {
    __containerType__ Node *
    public:

    void constructor(struct RigidBody *rigidBody) {
        this->object = rigidBody;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void get_collisions_shapes(Node ****shapes, u8 **length) {
        RigidBody *rigidBody = (RigidBody *) this->object;
        *length = &rigidBody->length;
        *shapes = &rigidBody->collisionsShapes;
    }

    void update(vec3 *pos, vec3 *rot, vec3 *scale, double delta) {
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

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
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

    void save(FILE *file) {
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

    void apply_impulse(float *impulse, float *torque, float *correction, float *momentOfInertia) {
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

    float get_velocity_norm() {
        RigidBody *rigidBody = (RigidBody *) this->object;
        return glm_vec3_norm(rigidBody->velocity);
    };

    /**
     * @brief Get the velocity of a node.
     * 
     * @param velocity Output vector to store the velocity.
     */

    void get_velocity(vec3 *velocity) {
        RigidBody *rigidBody = (RigidBody *) this->object;
        glm_vec3_copy(rigidBody->velocity, *velocity);
    };

    /**
     * @brief Get the mass of a node.
     * 
     * @param mass Output pointer to store the mass.
     */

    void get_mass(float * mass) {
        RigidBody *rigidBody = (RigidBody *) this->object;
        (*mass) = rigidBody->mass;
    }

    /**
     * @brief Get the center of mass of a node.
     * 
     * @param com Output vector to store the center of mass.
     */

    void get_center_of_mass(vec3 *com) {
        RigidBody *rigidBody = (RigidBody *) this->object;
        glm_vec3_copy(rigidBody->centerOfMass, *com);
    }
    
}
