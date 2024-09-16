#include "../types.h"
#include "../math/math_util.h"
#include "../io/shader.h"

#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    Vec3f pos,dir,rot,targetRot,targetPos;
} Camera;

void init_camera(Camera *c);
void camera_projection(Camera *c, Shader shaders[]);

#endif