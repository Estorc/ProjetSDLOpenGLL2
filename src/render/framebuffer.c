#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "framebuffer.h"
#include "../node.h"
#include "../window.h"


/**
 * Creates a multisampled framebuffer object (FBO) for rendering and an intermediate framebuffer for post-processing.
 * 
 * @param framebuffer {FBO*} Pointer to the framebuffer object to be created for multisampling.
 * @param intermediateFBO {FBO*} Pointer to the framebuffer object to be created for intermediate post-processing.
 * @param screenTexture {TextureMap*} Pointer to the texture map that will store the color attachment for the intermediate framebuffer.
 * 
 * This function configures two framebuffers:
 * 1. A multisampled framebuffer that uses multisampling for anti-aliasing. It includes a color attachment that is a multisampled texture 
 *    and a renderbuffer for depth and stencil attachments. The multisampling is set to 4 samples.
 * 2. An intermediate framebuffer that contains a regular color attachment texture for post-processing effects.
 * 
 * The function checks the completeness of both framebuffers after their configurations. If any framebuffer is not complete, an error message 
 * is printed to the console.
 */

void create_msaa_framebuffer(FBO *framebuffer, FBO *intermediateFBO, TextureMap *screenTexture) {
    // configure MSAA framebuffer
    // --------------------------
    glGenFramebuffers(1, framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
    // create a multisampled color attachment texture
    unsigned int textureColorBufferMultiSampled;
    glGenTextures(1, &textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure second post-processing framebuffer
    glGenFramebuffers(1, intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, *intermediateFBO);
    // create a color attachment texture
    glGenTextures(1, screenTexture);
    glBindTexture(GL_TEXTURE_2D, *screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *screenTexture, 0);	// we only need a color buffer

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}  