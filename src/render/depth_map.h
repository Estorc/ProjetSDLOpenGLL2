#include "../io/model.h"
#include "framebuffer.h"
#ifndef DEPTH_MAP_H
#define DEPTH_MAP_H

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024
#define MAX_SHADOW 100

typedef struct DepthMap {
    FBO frameBuffer;
    TextureMap texture;
    GLuint ubo;
} DepthMap;

struct WorldShaders;

#endif


void create_depthmap(DepthMap *depthMap, struct WorldShaders *shaders);