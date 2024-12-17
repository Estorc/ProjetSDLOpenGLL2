#ifndef RENDER_H
#define RENDER_H

typedef struct WorldShaders {
    Shader render;
    Shader depth;
    Shader screen;
    Shader skybox;
    Shader gui;
} WorldShaders;

struct Window;
struct DepthMap;
struct Camera;
struct Node;
struct MSAA;
struct Mesh;

void configure_global_lighting(struct Window *window, struct Node *root, struct Camera *c, WorldShaders *shaders);
void draw_screen(struct Window *window, struct Node *scene, struct Camera *c, WorldShaders *shaders, struct DepthMap *depthMap, struct MSAA *msaa, struct Mesh *screenPlane);
#endif