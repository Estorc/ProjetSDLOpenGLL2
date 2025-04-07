/**
 * @file
 * @brief This file contains the implementation of the PhysicalNode class.
 *
 * This source file contains the implementation of the PhysicalNode class, which is responsible for handling
 * the physics-related properties and behaviors of nodes in the application. It includes functions for
 * initializing, updating, and managing the physical properties of nodes, such as position, velocity, and
 * collision detection.
 *
 * It is designed to be used in the context of a larger application that requires physics simulation for
 * various objects or entities within a scene.
 *
 * @author Loup Picault
 * @date 2023-10-31
 */

#include <raptiquax.h>
#include <classes/classes.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <storage/node.h>
#include <io/scene_loader.h>
#include <render/lighting.h>
#include <buffer.h>

/**
 * @ingroup Classes Classes
 * @{
 */

class PhysicalNode : public Node {
    __containerType__ Node *
    public:

    /**
     * @brief Frees the resources allocated for the PhysicalNode instance.
     *
     * This function is responsible for releasing any memory or resources that were allocated for the PhysicalNode instance. It should be called when the PhysicalNode is no longer needed to prevent memory leaks.
     */
    void free() {
        u8 *length;
        Node ***shapes;
        this::get_collisions_shapes(&shapes, &length);
        for (int j = 0; j < *length; j++) {
            free((*shapes)[j]);
        }
        free((*shapes));
        SUPER(free);
    }
}

