/**
 * @file
 * @brief Framebuffer class implementation
 *
 * This file contains the implementation of the framebuffer class, which is used
 * to manage framebuffer objects in an OpenGL context. It provides functions to
 * create, bind, unbind, and delete framebuffer objects, as well as to attach
 * textures and renderbuffers to them.
 *
 * Detailed description:
 * The framebuffer class is essential for off-screen rendering in OpenGL. It allows
 * rendering to textures and renderbuffers instead of directly to the screen. This
 * is useful for various advanced rendering techniques such as shadow mapping, 
 * post-processing effects, and more. The class ensures proper management of 
 * framebuffer resources and provides a convenient interface for their usage.
 *
 * Author: Loup Picault
 * Date: 2023-10-06
 */

#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Framebuffer : public Node {
    __containerType__ Node *
    public:

    void constructor(FrameBuffer *frameBuffer) {
        this->object = frameBuffer;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void load() {
        FrameBuffer *msaa_framebuffer;
        msaa_framebuffer = malloc(sizeof(FrameBuffer));

        this::constructor(msaa_framebuffer);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
