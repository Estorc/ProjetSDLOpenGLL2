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

/**
 * @ingroup Classes Classes
 * @{
 */
class KinematicBody : public Body {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the kinematic body class.
     *
     * This function initializes a kinematic body with the given velocity.
     *
     * @param velocity Vec3 representing the initial velocity of the kinematic body.
     */
    void constructor(vec3 velocity) {
        this->type = __type__;

        KinematicBody *kinematicBody;
        kinematicBody = malloc(sizeof(KinematicBody));
        kinematicBody->collisionsShapes = NULL;
        kinematicBody->length = 0;
        glm_vec3_copy(velocity, kinematicBody->velocity);
        POINTER_CHECK(kinematicBody);

        this->object = kinematicBody;
        SUPER(initialize_node);
    }

    /**
     * @brief Retrieves the length of a kinematic body.
     *
     * This function assigns the length of a kinematic body to the provided pointer.
     *
     * @param length A pointer to an unsigned 8-bit integer where the length will be stored.
     */
    void get_length(u8 *length) {
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        *length = kinematicBody->length;
    }

    /**
     * @brief Retrieves the collision shapes and their count.
     *
     * This function populates the provided pointers with the collision shapes and their respective count.
     *
     * @param[out] shapes A pointer to a 4-dimensional array of Node pointers where the collision shapes will be stored.
     * @param[out] length A pointer to an array of unsigned 8-bit integers where the count of collision shapes will be stored.
     */
    void get_collisions_shapes(Node ****shapes, u8 **length) {
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        *length = &kinematicBody->length;
        *shapes = &kinematicBody->collisionsShapes;
    }

    /**
     * @brief Updates the global position of a kinematic body.
     *
     * This function updates the global position of a kinematic body based on the provided position, rotation, and scale vectors.
     *
     * @param pos Vec3 structure representing the position of the kinematic body.
     * @param rot Vec3 structure representing the rotation of the kinematic body.
     * @param scale Vec3 structure representing the scale of the kinematic body.
     */
    void update_global_position(vec3 pos, vec3 rot, vec3 scale) {
        SUPER(update_global_position, pos, rot, scale);
        KinematicBody *kinematicBody = (KinematicBody *) this->object;
        for (int i = 0; i < kinematicBody->length; i++) {
            (kinematicBody->collisionsShapes[i])::update_global_position(pos, rot, scale);
            glm_vec3_copy(this->globalPos, pos);
            glm_vec3_copy(this->globalRot, rot);
            glm_vec3_copy(this->globalScale, scale);
        }
    }

    /**
     * @brief Updates the position, rotation, and scale of a kinematic body.
     *
     * This function updates the position, rotation, and scale vectors of a kinematic body
     * based on the given delta time. It is typically used in a physics simulation to
     * update the state of the body over time.
     *
     * @param pos Vec3 structure representing the position of the body.
     * @param rot Vec3 structure representing the rotation of the body.
     * @param scale Vec3 structure representing the scale of the body.
     * @param delta The time delta used to update the body's state.
     */
    void update(vec3 pos, vec3 rot, vec3 scale, float delta) {
        KinematicBody *kinematicBody = (KinematicBody *) this->object;

        vec3 deltaVelocity;
        glm_vec3_scale(kinematicBody->velocity, 62.5f*delta, deltaVelocity);
        glm_vec3_add(this->pos, deltaVelocity, this->pos);
        this::update_global_position(pos, rot, scale);

        for (int i = 0; i < kinematicBody->length; i++) {
            check_collisions(kinematicBody->collisionsShapes[i]);
        }
        memcpy(&Game.buffers->collisionBuffer.collisionsShapes[Game.buffers->collisionBuffer.index], kinematicBody->collisionsShapes, kinematicBody->length * sizeof(kinematicBody->collisionsShapes[0]));
        Game.buffers->collisionBuffer.index += kinematicBody->length;
    }

    /**
     * @brief Loads data from a file and initializes the provided Camera, Script, and Node objects.
     *
     * This function reads data from the specified file and uses it to initialize the provided
     * Camera, Script, and Node objects. The function assumes that the file is already opened
     * and ready for reading.
     *
     * @param file A pointer to the FILE object from which data will be read.
     * @param c A double pointer to a Camera object that will be initialized with the data read from the file.
     * @param scripts A pointer to a Script object that will be initialized with the data read from the file.
     * @param editor A pointer to a Node object that will be initialized with the data read from the file.
     */
    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        vec3 velocity;
        int children_count = 0;
        if (file) {
            fscanf(file,"(%g,%g,%g,%d)\n", 
                &velocity[0], &velocity[1], &velocity[2], 
                &children_count);
        } else {
            glm_vec3_zero(velocity);
        }

        this::constructor(velocity);

        KinematicBody *kinematicBody = this->object;

        kinematicBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        Game.buffers->collisionBuffer.length += children_count;
        POINTER_CHECK(kinematicBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            kinematicBody->collisionsShapes[kinematicBody->length++] = child;
            child->parent = this;
        }
    }

    /**
     * @brief Saves the state of the kinematic body to a file.
     *
     * This function writes the current state of the kinematic body to the specified file.
     *
     * @param file A pointer to the file where the state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        KinematicBody *kinematicBody = (KinematicBody*) this->object;
        u8 collisionsLength = kinematicBody->length;
        fprintf(file, "(%g,%g,%g,%d)",
        kinematicBody->velocity[0], kinematicBody->velocity[1], kinematicBody->velocity[2], 
        collisionsLength);
    }

    /**
     * @brief Applies an impulse to the kinematic body.
     *
     * This function applies a given impulse and torque to the kinematic body and performs any necessary corrections.
     *
     * @param impulse Vec3 representing the impulse to be applied.
     * @param torque Vec3 representing the torque to be applied.
     * @param correction Vec3 representing the correction to be applied.
     */
    void apply_impulse(vec3 impulse, vec3 torque, vec3 correction) {
        UNUSED(torque);
        KinematicBody *kinematicBody = this->object;

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
        KinematicBody *kinematicBody = this->object;
        return glm_vec3_norm(kinematicBody->velocity);
    };

    /**
     * @brief Get the velocity of a node.
     * 
     * @param velocity Output vector to store the velocity.
     */
    void get_velocity(vec3 velocity) {
        KinematicBody *kinematicBody = this->object;
        glm_vec3_copy(kinematicBody->velocity, velocity);
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
    void get_center_of_mass(vec3 com) {
        glm_vec3_zero(com);
    }
}

