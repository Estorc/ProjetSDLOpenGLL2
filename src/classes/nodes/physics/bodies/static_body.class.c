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

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

/**
 * @ingroup Classes Classes
 * @{
 */
class StaticBody : public Body {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the static body class.
     *
     * This function initializes a static body with the given normal force.
     *
     * @param forcedNormal A pointer to a float representing the normal force to be applied to the static body.
     */
    void constructor(float *forcedNormal) {
        this->type = __type__;

        StaticBody *staticBody;
        staticBody = malloc(sizeof(StaticBody));
        staticBody->length = 0;
        POINTER_CHECK(staticBody);
        glm_vec3_copy(forcedNormal, staticBody->forcedNormal);

        this->object = staticBody;
        SUPER(initialize_node);
    }

    /**
     * @brief Retrieves the collision shapes and their lengths.
     *
     * This function populates the provided pointers with the collision shapes and their corresponding lengths.
     *
     * @param shapes A pointer to a 4-dimensional array of Node pointers. This will be populated with the collision shapes.
     * @param length A pointer to an array of unsigned 8-bit integers. This will be populated with the lengths of the collision shapes.
     */
    void get_collisions_shapes(Node ****shapes, u8 **length) {
        StaticBody *staticBody = (StaticBody *) this->object;
        *length = &staticBody->length;
        *shapes = &staticBody->collisionsShapes;
    }

    /**
     * @brief Updates the global position of a static body.
     *
     * This function updates the global position of a static body based on the provided position, rotation, and scale vectors.
     *
     * @param pos A pointer to a vec3 structure representing the position of the static body.
     * @param rot A pointer to a vec3 structure representing the rotation of the static body.
     * @param scale A pointer to a vec3 structure representing the scale of the static body.
     */
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

    /**
     * @brief Updates the position, rotation, and scale of a static body.
     *
     * This function updates the transformation properties of a static body in the physics engine.
     *
     * @param pos Pointer to a vec3 structure representing the position of the static body.
     * @param rot Pointer to a vec3 structure representing the rotation of the static body.
     * @param scale Pointer to a vec3 structure representing the scale of the static body.
     */
    void update(vec3 *pos, vec3 *rot, vec3 *scale) {
        StaticBody *staticBody = (StaticBody *) this->object;

        this::update_global_position(pos, rot, scale);

        for (int i = 0; i < staticBody->length; i++) {
            check_collisions(staticBody->collisionsShapes[i]);
        }
        memcpy(&Game.buffers->collisionBuffer.collisionsShapes[Game.buffers->collisionBuffer.index], staticBody->collisionsShapes, staticBody->length * sizeof(staticBody->collisionsShapes[0]));
        Game.buffers->collisionBuffer.index += staticBody->length;
    }

    /**
     * @brief Loads data from a file and initializes the provided Camera, Script, and Node structures.
     *
     * @param file A pointer to the FILE object from which data will be read.
     * @param c A double pointer to a Camera structure that will be initialized with data from the file.
     * @param scripts A pointer to a Script structure that will be initialized with data from the file.
     * @param editor A pointer to a Node structure that will be initialized with data from the file.
     */
    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        vec3 forcedNormal;
        int children_count = 0;
        if (file) {
            fscanf(file,"(%d", &children_count);
            if (fgetc(file) != ')') {
                fscanf(file, "%f,%f,%f)\n", &forcedNormal[0], &forcedNormal[1], &forcedNormal[2]);
            } else {
                fscanf(file, "\n");
                memset(forcedNormal, 0, sizeof(forcedNormal));
            }
        }
        this::constructor(forcedNormal);

        staticBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        Game.buffers->collisionBuffer.length += children_count;
        POINTER_CHECK(staticBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            staticBody->collisionsShapes[staticBody->length++] = child;
            child->parent = this;
        }
    }

    /**
     * @brief Saves the current state of the static body to a file.
     *
     * This function writes the relevant data of the static body to the provided file.
     *
     * @param file A pointer to a FILE object that identifies the file to which the data will be written.
     */
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

    /**
     * @brief Retrieves the collision normal vector.
     *
     * This function calculates and returns the normal vector of a collision.
     * The normal vector is a unit vector that is perpendicular to the surface
     * at the point of collision.
     *
     * @param[out] normal A pointer to a float array where the normal vector
     *                    will be stored. The array should have enough space
     *                    to store the components of the normal vector.
     */
    void get_collision_normal(float *normal) {
        StaticBody *staticBody = (StaticBody *) this->object;
        glm_vec3_copy(staticBody->forcedNormal, normal);
    }

}

