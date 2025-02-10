/**
 * @file
 * @brief This file defines the implementation of static bodies in the physics engine.
 *
 * This file contains the implementation of static bodies used in the physics engine.
 * Static bodies are objects that do not move and are used to represent immovable objects
 * in the simulation, such as walls or floors. The code provides functions to initialize,
 * update, and manage these static bodies within the physics engine.
 * 
 * Detailed description:
 * The static body class includes functions for creating and initializing static bodies,
 * setting their properties, and integrating them into the broader physics simulation.
 * These bodies are essential for defining the environment and boundaries within which
 * dynamic bodies interact.
 * 
 * @author Loup Picault
 * @date 2023-10-31
 */

#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class StaticBody : public Body {
    __containerType__ Node *
    public:

    void constructor(struct StaticBody *staticBody) {
        this->object = staticBody;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void get_collisions_shapes(Node ****shapes, u8 **length) {
        StaticBody *staticBody = (StaticBody *) this->object;
        *length = &staticBody->length;
        *shapes = &staticBody->collisionsShapes;
    }

    void update_global_position(vec3 *pos, vec3 *rot, vec3 *scale) {
        SUPER(update_global_position, pos, rot, scale);
        StaticBody *staticBody = (StaticBody *) this->object;
        for (int i = 0; i < staticBody->length; i++) {
            (staticBody->collisionsShapes[i])::update_global_position(pos, rot, scale);
            glm_vec3_copy(this->globalPos, *pos);
            glm_vec3_copy(this->globalRot, *rot);
            glm_vec3_copy(this->globalScale, *scale);
        }
    }

    void update(vec3 *pos, vec3 *rot, vec3 *scale) {
        StaticBody *staticBody = (StaticBody *) this->object;

        this::update_global_position(pos, rot, scale);

        for (int i = 0; i < staticBody->length; i++) {
            check_collisions(staticBody->collisionsShapes[i]);
        }
        memcpy(&buffers.collisionBuffer.collisionsShapes[buffers.collisionBuffer.index], staticBody->collisionsShapes, staticBody->length * sizeof(staticBody->collisionsShapes[0]));
        buffers.collisionBuffer.index += staticBody->length;
    }

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        StaticBody *staticBody;
        staticBody = malloc(sizeof(StaticBody));
        staticBody->length = 0;
        int children_count = 0;
        POINTER_CHECK(staticBody);
        if (file) {
            fscanf(file,"(%d", &children_count);
            if (fgetc(file) != ')') {
                fscanf(file, "%f,%f,%f)\n", &staticBody->forcedNormal[0], &staticBody->forcedNormal[1], &staticBody->forcedNormal[2]);
            } else {
                fscanf(file, "\n");
                memset(staticBody->forcedNormal, 0, sizeof(staticBody->forcedNormal));
            }
        }
        this->type = __type__;
        this::constructor(staticBody);

        staticBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(staticBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            staticBody->collisionsShapes[staticBody->length++] = child;
            child->parent = this;
        }
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        StaticBody *staticBody = (StaticBody*) this->object;
        u8 collisionsLength = staticBody->length;
        fprintf(file, "(%d)", collisionsLength);
    }

    /**
     * @brief Get the velocity norm of a node.
     * 
     * @return The velocity norm of the node.
     */

    float get_velocity_norm() {
        return 0.0f;
    };

    /**
     * @brief Get the velocity of a node.
     * 
     * @param velocity Output vector to store the velocity.
     */

    void get_velocity(vec3 *velocity) {
        glm_vec3_zero(*velocity);
    };

    /**
     * @brief Get the mass of a node.
     * 
     * @param mass Output pointer to store the mass.
     */

    void get_mass(float * mass) {
        (*mass) = INFINITY;
    }

    /**
     * @brief Get the center of mass of a node.
     * 
     * @param com Output vector to store the center of mass.
     */

    void get_center_of_mass(float *com) {
        glm_vec3_zero(com);
    }



    void get_collision_normal(float *normal) {
        StaticBody *staticBody = (StaticBody *) this->object;
        glm_vec3_copy(staticBody->forcedNormal, normal);
    }

}
