#include "raptiquax.h"
#include "math/math_util.h"
#include "physics/bodies.h"
#include "render/lighting.h"
#include "buffer.h"

void init_buffers() {
    Game.buffers->collisionBuffer.collisionsShapes = NULL;
    Game.buffers->lightingBuffer.lightings = NULL;
}

void free_buffers() {
    free(Game.buffers->collisionBuffer.collisionsShapes);
    free(Game.buffers->lightingBuffer.lightings);
    PRINT_INFO("Free buffers!\n");
}