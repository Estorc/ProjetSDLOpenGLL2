#ifndef BUFFER_H
#define BUFFER_H

struct LightingBuffer;
struct CollisionBuffer;

typedef struct BufferCollection {
    struct LightingBuffer lightingBuffer;
    struct CollisionBuffer collisionBuffer;
} BufferCollection;

extern BufferCollection buffers;

#endif

void init_buffers();
void free_buffers();