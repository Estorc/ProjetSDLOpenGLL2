#include <raptiquax.h>
#include <math/math_util.h>
#include <io/model.h>
#include <framebuffer.h>
#include <storage/node.h>
#include <window.h>
#include <memory.h>


// Helper function to create a texture
void create_texture(TextureMap *tex, GLint attachment, GLenum internalFormat, GLenum format, int width, int height) {
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, *tex, 0);
};


static CFBO intermediateFBO[2] = {0};
static CFBO *currentIFBO = 0;

void swap_intermediate_fbo() {
    if (currentIFBO == &intermediateFBO[0]) {
        currentIFBO = &intermediateFBO[1];
    } else {
        currentIFBO = &intermediateFBO[0];
    }
}

TextureMap get_intermediate_texture() {
    if (currentIFBO == &intermediateFBO[0]) {
        return intermediateFBO[1].texture;
    } else {
        return intermediateFBO[0].texture;
    }
}

void use_intermediate_fbo() {
    swap_intermediate_fbo();
    use_fbo(GL_FRAMEBUFFER, currentIFBO->framebuffer);
}

void create_intermediate_fbo() {
    create_cfbo(&intermediateFBO[0]);
    create_cfbo(&intermediateFBO[1]);
}

void free_intermediate_fbo() {
    free_cfbo(&intermediateFBO[0]);
    free_cfbo(&intermediateFBO[1]);
}

void resize_intermediate_fbo() {
    resize_cfbo(&intermediateFBO[0]);
    resize_cfbo(&intermediateFBO[1]);
}

static FBO currentFBO = 0;

void use_fbo(GLenum mode, FBO fbo) {
    glBindFramebuffer(mode, fbo);
    currentFBO = fbo;
}

FBO get_fbo() {
    return currentFBO;
}

void create_cfbo(CFBO *cFbo) {
    int window_width, window_height;
    get_resolution(&window_width, &window_height);
    if (window_width <= 0 || window_height <= 0) {
        PRINT_WARNING("Invalid window dimensions: width=%d, height=%d\n", window_width, window_height);
        return;
    }

    PRINT_INFO("Creating CFBO framebuffer\n");
    PRINT_INFO("Window width: %d\n", window_width);
    PRINT_INFO("Window height: %d\n", window_height);


    glGenFramebuffers(1, &cFbo->framebuffer);
    use_fbo(GL_FRAMEBUFFER, cFbo->framebuffer);

    // Create texture to store FBO result
    glGenTextures(1, &cFbo->texture);
    glBindTexture(GL_TEXTURE_2D, cFbo->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Attach texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cFbo->texture, 0);
    
    // Check if FBO is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        PRINT_ERROR("FBO failed!\n");
    }
    
    // Unbind FBO (back to default framebuffer)
    use_fbo(GL_FRAMEBUFFER, 0);
}

void free_cfbo(CFBO *cFbo) {
    glDeleteFramebuffers(1, &cFbo->framebuffer);
    glDeleteTextures(1, &cFbo->texture);
}

void resize_cfbo(CFBO *cFbo) {
    free_cfbo(cFbo);
    create_cfbo(cFbo);
}

void create_dfbo(DFBO *dfbo) {
    int window_width, window_height;
    get_resolution(&window_width, &window_height);
    if (window_width <= 0 || window_height <= 0) {
        PRINT_WARNING("Invalid window dimensions: width=%d, height=%d\n", window_width, window_height);
        return;
    }

    PRINT_INFO("Creating DFBO framebuffer\n");
    PRINT_INFO("Window width: %d\n", window_width);
    PRINT_INFO("Window height: %d\n", window_height);
    // Create and bind the main framebuffer
    glGenFramebuffers(1, &dfbo->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, dfbo->framebuffer);

    // Create and attach textures
    create_texture(&dfbo->textureColorBuffer, 0, GL_RGBA32F, GL_RGBA, window_width, window_height);
    create_texture(&dfbo->texturePositionBuffer, 1, GL_RGBA32F, GL_RGBA, window_width, window_height);
    create_texture(&dfbo->textureNormalBuffer, 2, GL_RGBA32F, GL_RGBA, window_width, window_height);
    create_texture(&dfbo->textureExtraComponent, 3, GL_RGBA32F, GL_RGBA, window_width, window_height);

    // Create depth attachment (optional)
    glGenRenderbuffers(1, &dfbo->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, dfbo->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_width, window_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dfbo->rbo);

    // Set draw buffers
    GLenum attachments[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3
    };
    glDrawBuffers(4, attachments);

    // Check for completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        PRINT_ERROR("DFBO Framebuffer is not complete!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void free_dfbo(DFBO *dfbo) {
    glDeleteFramebuffers(1, &dfbo->framebuffer);
    glDeleteTextures(1, &dfbo->textureColorBuffer);
    glDeleteTextures(1, &dfbo->textureNormalBuffer);
    glDeleteTextures(1, &dfbo->texturePositionBuffer);
    glDeleteTextures(1, &dfbo->textureExtraComponent);
}

void resize_dfbo(DFBO *dfbo) {
    free_dfbo(dfbo);
    create_dfbo(dfbo);
}


void create_msaa(MSAA *msaa) {
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
    use_fbo(GL_FRAMEBUFFER, msaa->framebuffer);
    // create a multisampled color attachment texture
    glGenTextures(1, &msaa->textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaa->textureColorBufferMultiSampled);

    GLint srgbCapable = 0;
    SDL_GL_GetAttribute(GL_ARB_framebuffer_sRGB, &srgbCapable);

    if (srgbCapable) {
        PRINT_INFO("sRGB framebuffer is supported!\n");
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_SRGB8, window_width, window_height, GL_TRUE);
    } else {
        PRINT_WARNING("sRGB framebuffer is NOT supported.\n");
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
    use_fbo(GL_FRAMEBUFFER, 0);

    // Configure second post-processing framebuffer
    glGenFramebuffers(1, &msaa->intermediateFBO);
    use_fbo(GL_FRAMEBUFFER, msaa->intermediateFBO);

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



void free_msaa(MSAA *msaa) {
    // Unbind any resources before deletion
    use_fbo(GL_FRAMEBUFFER, 0);
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





void resize_msaa(MSAA *msaa) {

    free_msaa(msaa);

    // Recreate the MSAA framebuffer
    create_msaa(msaa);
}