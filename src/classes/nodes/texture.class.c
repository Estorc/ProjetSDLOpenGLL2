/**
 * @file
 * @brief This file contains the implementation of texture-related functionalities.
 *
 * This file provides the necessary functions and structures to handle textures
 * within the project. It includes loading, binding, and managing textures for
 * rendering purposes.
 *
 * @author Loup Picault
 * @date 2023-10-31
 */

#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Texture : public Node {
    __containerType__ Node *
    public:

    void constructor(TextureMap *texture) {
        this->object = texture;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void load() {

    }

    void save() {

    }




    
}
