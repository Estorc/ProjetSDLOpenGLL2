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

#include "raptiquax.h"
#include "classes/classes.h"
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

    void update_global_position(vec3 *pos, vec3 *rot, vec3 *scale) {
        SUPER(update_global_position, pos, rot, scale);
        RigidBody *rigidBody = (RigidBody *) this->object;
        for (int i = 0; i < rigidBody->length; i++) {
            (rigidBody->collisionsShapes[i])::update_global_position(pos, rot, scale);
            glm_vec3_copy(this->globalPos, *pos);
            glm_vec3_copy(this->globalRot, *rot);
            glm_vec3_copy(this->globalScale, *scale);
        }
    }

    void apply_torque(float *torque) {
        RigidBody *rigidBody = (RigidBody *) this->object;
        // Apply torque

        // 1. Compute local inertia tensor (example for a box)
        mat3 localInertiaTensor;
        float height = 1.6f;
        float width = 0.5f;
        float depth = 0.5f;
        float height2 = sqr(height);
        float width2 = sqr(width);
        float depth2 = sqr(depth);
        localInertiaTensor[0][0] = (1.0f / 12.0f) * rigidBody->mass * (height2 + depth2);
        localInertiaTensor[1][1] = (1.0f / 12.0f) * rigidBody->mass * (width2 + depth2);
        localInertiaTensor[2][2] = (1.0f / 12.0f) * rigidBody->mass * (width2 + height2);


        // 2. Get rotation matrix
        mat3 rotationMatrix;
        mat4 rotationMatrix4;
        vec3 radiansRot;
        glm_vec3_scale(this->globalRot, PI/180.0f, radiansRot);
        glm_euler(radiansRot, rotationMatrix4);
        glm_mat4_pick3(rotationMatrix4, rotationMatrix);

        // 3. Transform to world space
        mat3 worldInertiaTensor;
        glm_mat3_mul(rotationMatrix, localInertiaTensor, worldInertiaTensor);
        glm_mat3_transpose(worldInertiaTensor);

        // 4. Compute inverse inertia tensor
        mat3 inverseInertiaTensor;
        glm_mat3_inv(worldInertiaTensor, inverseInertiaTensor);

        // 5. Apply torque to get angular acceleration
        glm_mat3_mulv(inverseInertiaTensor, torque, torque);

        glm_vec3_scale(torque, 3.0f, torque);
        glm_vec3_add(rigidBody->angularVelocity, torque, rigidBody->angularVelocity);
    }

    void update(vec3 *pos, vec3 *rot, vec3 *scale, double delta) {
        RigidBody *rigidBody = (RigidBody *) this->object;

        vec3 gravity;
        glm_vec3_scale(rigidBody->gravity, delta, gravity);
        glm_vec3_add(rigidBody->velocity, gravity, rigidBody->velocity);
        glm_vec3_scale(rigidBody->velocity, rigidBody->friction * (1.0-delta), rigidBody->velocity);
        glm_vec3_add(this->pos, rigidBody->velocity, this->pos);

        glm_vec3_scale(rigidBody->angularVelocity, 0.98f, rigidBody->angularVelocity);
        if (glm_vec3_norm2(rigidBody->angularVelocity) < 0.01f) 
            glm_vec3_zero(rigidBody->angularVelocity);

        // Limit angular velocity to prevent it from getting too high
        float maxAngularVelocity = 10.0f; // Define a maximum angular velocity
        if (glm_vec3_norm(rigidBody->angularVelocity) > maxAngularVelocity) {
            glm_vec3_normalize(rigidBody->angularVelocity);
            glm_vec3_scale(rigidBody->angularVelocity, maxAngularVelocity, rigidBody->angularVelocity);
        }


        vec3 degreeAngularVelocity;
        glm_vec3_scale(rigidBody->angularVelocity, delta, degreeAngularVelocity);
        glm_vec3_scale(degreeAngularVelocity, 180.0f/PI, degreeAngularVelocity);
        glm_vec3_add(this->rot, rigidBody->angularVelocity, this->rot);

        this::update_global_position(pos, rot, scale);

        for (int i = 0; i < rigidBody->length; i++) {
            check_collisions(rigidBody->collisionsShapes[i]);
        }
        memcpy(&Game.buffers->collisionBuffer.collisionsShapes[Game.buffers->collisionBuffer.index], rigidBody->collisionsShapes, rigidBody->length * sizeof(rigidBody->collisionsShapes[0]));
        Game.buffers->collisionBuffer.index += rigidBody->length;
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
        Game.buffers->collisionBuffer.length += children_count;
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

    void apply_impulse(float *impulse, float *torque, float *correction) {
        RigidBody *rigidBody = (RigidBody *) this->object;

        // Change velocity
        glm_vec3_add(rigidBody->velocity, impulse, rigidBody->velocity);

        // Move shape A out of shape B
        glm_vec3_sub(this->pos, correction, this->pos);
        glm_vec3_sub(this->globalPos, correction, this->globalPos);
        for (int i = 0; i < rigidBody->length; i++) {
            glm_vec3_sub(rigidBody->collisionsShapes[i]->globalPos, correction, rigidBody->collisionsShapes[i]->globalPos);
        }

        this::apply_torque(torque);
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
