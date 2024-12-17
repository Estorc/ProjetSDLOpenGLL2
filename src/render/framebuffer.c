#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "framebuffer.h"
#include "../storage/node.h"
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

void create_msaa_framebuffer(MSAA *msaa) {
    int window_width, window_height;
    get_resolution(&window_width, &window_height);
    if (window_width <= 0 || window_height <= 0) {
        printf("Invalid window dimensions: width=%d, height=%d\n", window_width, window_height);
        return;
    }

    printf("Creating MSAA framebuffer\n");
    printf("Window width: %d\n", window_width);
    printf("Window height: %d\n", window_height);

    // configure MSAA framebuffer
    // --------------------------
    glGenFramebuffers(1, &msaa->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, msaa->framebuffer);
    // create a multisampled color attachment texture
    glGenTextures(1, &msaa->textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaa->textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, window_width, window_height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaa->textureColorBufferMultiSampled, 0);
    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    glGenRenderbuffers(1, &msaa->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, msaa->rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, window_width, window_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaa->rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
        glDeleteFramebuffers(1, &msaa->framebuffer);
        glDeleteRenderbuffers(1, &msaa->rbo);
        glDeleteTextures(1, &msaa->textureColorBufferMultiSampled);
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Configure second post-processing framebuffer
    glGenFramebuffers(1, &msaa->intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, msaa->intermediateFBO);

    // Color attachment texture
    glGenTextures(1, &msaa->screenTexture);
    glBindTexture(GL_TEXTURE_2D, msaa->screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, msaa->screenTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!\n");
        glDeleteFramebuffers(1, &msaa->intermediateFBO);
        glDeleteTextures(1, &msaa->screenTexture);
        return;
    }
}  



void free_msaa_framebuffer(MSAA *msaa) {
    // Unbind any resources before deletion
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    // Delete existing framebuffers and textures
    glDeleteFramebuffers(1, &msaa->framebuffer);
    glDeleteFramebuffers(1, &msaa->intermediateFBO);
    glDeleteRenderbuffers(1, &msaa->rbo);
    glDeleteTextures(1, &msaa->textureColorBufferMultiSampled);
    glDeleteTextures(1, &msaa->screenTexture);
}





void resize_msaa_framebuffer(MSAA *msaa) {

    free_msaa_framebuffer(msaa);

    // Recreate the MSAA framebuffer
    create_msaa_framebuffer(msaa);
}