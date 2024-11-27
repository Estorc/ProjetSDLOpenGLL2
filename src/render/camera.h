#include "../types.h"
#include "../math/math_util.h"
#include "../io/shader.h"

#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera {
    Vec3f pos,dir,rot;
} Camera;

struct WorldShaders;
void init_camera(Camera *c);
void camera_projection(Camera *c, struct WorldShaders *shaders);

#endif