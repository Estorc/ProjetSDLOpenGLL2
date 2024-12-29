#include "../io/model.h"
#include "framebuffer.h"
#ifndef DEPTH_MAP_H
#define DEPTH_MAP_H

#define SHADOW_WIDTH 4096
#define SHADOW_HEIGHT 4096
#define MAX_SHADOW 25

typedef struct DepthMap {
    FBO frameBuffer;
    TextureMap texture;
    GLuint ubo;
} DepthMap;

struct WorldShaders;

#endif


void create_depthmap(DepthMap *depthMap, struct WorldShaders *shaders);