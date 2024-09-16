#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../types.h"
#include "../math/math_util.h"
#include "depth_map.h"


/**
 * Creates a depth map for shadow mapping by setting up a framebuffer and depth texture.
 * 
 * @param depthMap {DepthMap*} Pointer to the DepthMap structure where the framebuffer and texture will be stored.
 * 
 * This function initializes a framebuffer object (FBO) and a texture to store depth information.
 * The depth texture is configured to store depth data with the specified width and height for shadows.
 * It sets various parameters for the depth texture, including filtering and wrapping options, and binds
 * the texture to the depth attachment of the framebuffer.
 * 
 * After setup, the framebuffer and texture references are stored in the provided DepthMap structure
 * for later use in rendering shadows.
 */

void create_depthmap(DepthMap *depthMap) {
    FBO depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);  

    TextureMap depthMapTexture;
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);  
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    depthMap->frameBuffer = depthMapFBO;
    depthMap->texture = depthMapTexture;
}  