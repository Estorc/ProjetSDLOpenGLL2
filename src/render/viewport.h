#ifndef VIEWPORT_H
#define VIEWPORT_H

typedef struct Viewport {
    struct Node *scene;
    struct Node *screenPlane;
} Viewport;

#endif

void init_viewport();