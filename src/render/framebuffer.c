#include "../raptiquax.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "framebuffer.h"
#include "../storage/node.h"
#include "../window.h"



void create_msaa_framebuffer(MSAA *msaa) {
    int window_width, window_height;
    get_resolution(&window_width, &window_height);
    if (window_width <= 0 || window_height <= 0) {
        PRINT_WARNING("Invalid window dimensions: width=%d, height=%d\n", window_width, window_height);
        return;
    }

    PRINT_INFO("Creating MSAA framebuffer\n");
    PRINT_INFO("Window width: %d\n", window_width);
    PRINT_INFO("Window height: %d\n", window_height);

    // configure MSAA framebuffer
    // --------------------------
    glGenFramebuffers(1, &msaa->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, msaa->framebuffer);
    // create a multisampled color attachment texture
    glGenTextures(1, &msaa->textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaa->textureColorBufferMultiSampled);

    GLint srgbCapable = 0;
    SDL_GL_GetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, &srgbCapable);
    
    glEnable(GL_FRAMEBUFFER_SRGB);  // Enable sRGB framebuffer correction

    if (srgbCapable) {
        printf("sRGB framebuffer is supported!\n");
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_SRGB, window_width, window_height, GL_TRUE);
    } else {
        printf("sRGB framebuffer is NOT supported.\n");
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, window_width, window_height, GL_TRUE);
    }

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaa->textureColorBufferMultiSampled, 0);
    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    glGenRenderbuffers(1, &msaa->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, msaa->rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, window_width, window_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaa->rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        PRINT_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, msaa->screenTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        PRINT_ERROR("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!\n");
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