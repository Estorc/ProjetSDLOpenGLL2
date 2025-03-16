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

/**
 * @ingroup Classes Classes
 * @{
 */

class RenderTarget : public Node {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the render target class.
     *
     * This function initializes a render target with the specified width and height.
     *
     * @param width The width of the render target.
     * @param height The height of the render target.
     */
    void constructor(int width, int height) {

        this->type = __type__;

        RenderTarget *renderTarget = malloc(sizeof(RenderTarget));

        this->object = renderTarget;

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

    /**
     * @brief Updates the render target.
     *
     * This function is responsible for updating the state of the render target.
     * It performs necessary operations to ensure the render target is up-to-date.
     */
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

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the given file and processes it accordingly.
     *
     * @param file A pointer to the FILE object that represents the file to be read.
     */
    void load(FILE *file) {
        int width, height;
        fscanf(file, "(%d,%d)", &width, &height);
        this::constructor(width, height);
    }

    /**
     * @brief Saves the current state to the specified file.
     *
     * This function writes the current state of the render target to the given
     * file. The file must be opened in a mode that allows writing.
     *
     * @param file A pointer to a FILE object that identifies the file to which
     * the state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        fprintf(file, "(%d,%d)", ((RenderTarget *) this->object)->w, ((RenderTarget *) this->object)->h);
    }

    /**
     * @brief Checks if the current target is a render target.
     *
     * This function sets the value of the provided boolean pointer to indicate
     * whether the current target is a render target.
     *
     * @param render_target A pointer to a boolean variable that will be set to true
     *                      if the current target is a render target, or false otherwise.
     */
    void is_render_target(bool *render_target) {
        *render_target = true;
    }

    /**
     * @brief Frees the resources allocated for the render target.
     *
     * This function is responsible for releasing any memory or resources
     * that were allocated for the render target. It should be called when
     * the render target is no longer needed to avoid memory leaks.
     */
    void free() {
        RenderTarget *renderTarget = (RenderTarget *) this->object;
        glDeleteFramebuffers(1, &renderTarget->fbo);
        glDeleteTextures(1, &renderTarget->texture);
        SUPER(free);
    }

    
}

