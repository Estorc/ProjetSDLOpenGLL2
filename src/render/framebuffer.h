#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef unsigned int FBO;
typedef FBO FrameBuffer;

#endif

void create_msaa_framebuffer(FBO *framebuffer, FBO *intermediateFBO, TextureMap *screenTexture);