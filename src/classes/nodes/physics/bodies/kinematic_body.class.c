/**
 * @file
 * @brief This file defines the implementation of kinematic bodies for the physics engine.
 *
 * This source file contains the implementation of kinematic bodies, which are used in the physics engine
 * to represent objects that move according to predefined paths or velocities, but are not affected by forces
 * like gravity or collisions. These bodies are essential for creating predictable and controlled movements
 * in the simulation.
 *
 * The kinematic bodies are designed to interact with other physical entities in the simulation, providing
 * a way to animate objects or create complex motion patterns without the need for dynamic physics calculations.
 *
 * @author Loup Picault
 * @date 2023-10-20
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

class KinematicBody : public Body {
    __containerType__ Node *
    public:

    void constructor(struct KinematicBody *kinematicBody) {
        this->object = kinematicBody;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void get_length(u8 *length) {
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        *length = kinematicBody->length;
    }

    void get_collisions_shapes(Node ****shapes, u8 **length) {
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        *length = &kinematicBody->length;
        *shapes = &kinematicBody->collisionsShapes;
    }

    void update_global_position(vec3 *pos, vec3 *rot, vec3 *scale) {
        SUPER(update_global_position, pos, rot, scale);
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        for (int i = 0; i < kinematicBody->length; i++) {
            (kinematicBody->collisionsShapes[i])::update_global_position(pos, rot, scale);
            glm_vec3_copy(this->globalPos, *pos);
            glm_vec3_copy(this->globalRot, *rot);
            glm_vec3_copy(this->globalScale, *scale);
        }
    }

    void update(vec3 *pos, vec3 *rot, vec3 *scale) {
        KinematicBody *kinematicBody = (KinematicBody *) this->object;

        glm_vec3_add(this->pos, kinematicBody->velocity, this->pos);
        this::update_global_position(pos, rot, scale);

        for (int i = 0; i < kinematicBody->length; i++) {
            check_collisions(kinematicBody->collisionsShapes[i]);
        }
        memcpy(&buffers.collisionBuffer.collisionsShapes[buffers.collisionBuffer.index], kinematicBody->collisionsShapes, kinematicBody->length * sizeof(kinematicBody->collisionsShapes[0]));
        buffers.collisionBuffer.index += kinematicBody->length;
    }

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
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

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        KinematicBody *kinematicBody = (KinematicBody*) this->object;
        u8 collisionsLength = kinematicBody->length;
        fprintf(file, "(%g,%g,%g,%d)",
        kinematicBody->velocity[0], kinematicBody->velocity[1], kinematicBody->velocity[2], 
        collisionsLength);
    }


    void apply_impulse(float *impulse, float *torque, float *correction) {
        UNUSED(torque);
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

    float get_velocity_norm() {
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        return glm_vec3_norm(kinematicBody->velocity);
    };

    /**
     * @brief Get the velocity of a node.
     * 
     * @param velocity Output vector to store the velocity.
     */

    void get_velocity(vec3 *velocity) {
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        glm_vec3_copy(kinematicBody->velocity, *velocity);
    };

    /**
     * @brief Get the mass of a node.
     * 
     * @param mass Output pointer to store the mass.
     */

    void get_mass(float * mass) {
        (*mass) = 100.0;
    }

    /**
     * @brief Get the center of mass of a node.
     * 
     * @param com Output vector to store the center of mass.
     */

    void get_center_of_mass(float *com) {
        glm_vec3_zero(com);
    }
}
