#ifndef RENDER_H
#define RENDER_H

typedef struct WorldShaders {
    Shader render;
    Shader depth;
    Shader screen;
} WorldShaders;

struct Window;
struct DepthMap;
struct Camera;
struct Node;

void configure_global_lighting(struct Window *window, struct Node *root, struct Camera *c, WorldShaders *shaders);
void draw_screen(struct Window *window, struct Node *viewportNode, struct Camera *c, WorldShaders *shaders, struct DepthMap *depthMap);
#endif