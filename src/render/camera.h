#include "../types.h"
#include "../math/math_util.h"
#include "../io/shader.h"

#ifndef CAMERA_H
#define CAMERA_H

struct Node;

typedef struct Camera {
    vec3 pos,rot;
} Camera;

struct WorldShaders;
void init_camera(Camera *c);
void camera_projection(Camera *c, struct WorldShaders *shaders);

#endif