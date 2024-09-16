#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "filter.h"
#include "framebuffer.h"
#include "../node.h"
#include "../window.h"


/**
 * Prepares for the anti-aliasing process by binding the appropriate framebuffer.
 * 
 * @param node {Node*} Pointer to the node containing framebuffer information for the anti-aliasing operation.
 * @param texture {TextureMap*} Pointer to the texture map that may be updated with the result of the anti-aliasing process.
 * 
 * This function binds the framebuffer specified in the first child of the node. 
 * This framebuffer is typically used for rendering the scene with multisampling 
 * enabled, allowing for smoother edges and reduced aliasing artifacts. 
 * The texture parameter is provided for potential updates in subsequent operations, 
 * though it is not modified in this specific function.
 */

void anti_aliasing_begin_script(Node *node, TextureMap *texture) {
    glBindFramebuffer(GL_FRAMEBUFFER, *((FrameBuffer *) node->children[0]->object));
}


/**
 * Performs the final step of the anti-aliasing process by blitting the framebuffer contents to the screen.
 * 
 * @param node {Node*} Pointer to the node containing framebuffer information for the anti-aliasing operation.
 * @param texture {TextureMap*} Pointer to the texture map that will be updated with the result of the anti-aliasing process.
 * 
 * This function binds the read and draw framebuffers as specified in the node's children and uses `glBlitFramebuffer` 
 * to copy the color buffer from the read framebuffer to the draw framebuffer. This operation effectively resolves 
 * the multisampled rendering into a single framebuffer, providing a smoother image.
 * 
 * The texture map pointed to by the `texture` parameter is updated to reflect the texture associated with the 
 * filter stored in the node. This is typically used to apply the result of the anti-aliasing operation for further processing.
 */

void anti_aliasing_end_script(Node *node, TextureMap *texture) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, *((FrameBuffer *) node->children[0]->object));
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *((FrameBuffer *) node->children[1]->object));
    glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    *texture = ((Filter *) node->object)->texture;
}