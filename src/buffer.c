#include "types.h"
#include "math/math_util.h"
#include "physics/bodies.h"
#include "render/lighting.h"
#include "buffer.h"

void init_buffers() {
    buffers.collisionBuffer.collisionsShapes = NULL;
    buffers.lightingBuffer.lightings = NULL;
}

void free_buffers() {
    free(buffers.collisionBuffer.collisionsShapes);
    free(buffers.lightingBuffer.lightings);
    printf("Free buffers!\n");
}