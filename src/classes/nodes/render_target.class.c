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

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class RenderTarget : public Node {
    __containerType__ Node *
    public:

    void constructor(int width, int height) {

        RenderTarget *renderTarget = malloc(sizeof(RenderTarget));

        this->object = renderTarget;
        this->type = __type__;

        renderTarget->w = width;
        renderTarget->h = height;

        renderTarget->mouse.x = 0;
        renderTarget->mouse.y = 0;
        renderTarget->mouse.lastX = 0;
        renderTarget->mouse.lastY = 0;
        renderTarget->mouse.dx = 0;
        renderTarget->mouse.dy = 0;
        renderTarget->mouse.scroll_x = 0;
        renderTarget->mouse.scroll_y = 0;
        renderTarget->mouse.pressed_button = 0;
        renderTarget->mouse.released_button = 0;
        renderTarget->mouse.active_button = 0;

        glGenFramebuffers(1, &renderTarget->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->fbo);
        
        // Create texture to store FBO result
        glGenTextures(1, &renderTarget->texture);
        glBindTexture(GL_TEXTURE_2D, renderTarget->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Attach texture to FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget->texture, 0);
        
        // Check if FBO is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("FBO failed!\n");
        }
        
        // Unbind FBO (back to default framebuffer)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        SUPER(initialize_node);
    }


    void update() {
        RenderTarget *renderTarget = (RenderTarget *) this->object;

        renderTarget->mouse.lastX = renderTarget->mouse.x;
        renderTarget->mouse.lastY = renderTarget->mouse.y;
        renderTarget->mouse.dx = Game.input->mouse.dx;
        renderTarget->mouse.dy = Game.input->mouse.dy;
        renderTarget->mouse.scroll_x = Game.input->mouse.scroll_x;
        renderTarget->mouse.scroll_y = Game.input->mouse.scroll_y;

        renderTarget->mouse.pressed_button = Game.input->mouse.pressed_button;
        renderTarget->mouse.released_button = Game.input->mouse.released_button;
        renderTarget->mouse.active_button = Game.input->mouse.active_button;
    }

    

    void load(FILE *file) {
        int width, height;
        fscanf(file, "(%d,%d)", &width, &height);
        this::constructor(width, height);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        fprintf(file, "(%d,%d)", ((RenderTarget *) this->object)->w, ((RenderTarget *) this->object)->h);
    }


    void is_render_target(bool *render_target) {
        *render_target = true;
    }

    void free() {
        RenderTarget *renderTarget = (RenderTarget *) this->object;
        glDeleteFramebuffers(1, &renderTarget->fbo);
        glDeleteTextures(1, &renderTarget->texture);
        SUPER(free);
    }

    
}
