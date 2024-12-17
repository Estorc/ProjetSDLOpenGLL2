#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef unsigned int FBO;
typedef FBO FrameBuffer;
typedef unsigned int RBO;

typedef struct MSAA {
    FBO framebuffer;
    FBO intermediateFBO;
    RBO rbo;
    TextureMap textureColorBufferMultiSampled;
    TextureMap screenTexture;
} MSAA;

#endif

void create_msaa_framebuffer(MSAA *msaa);
void resize_msaa_framebuffer(MSAA *msaa);