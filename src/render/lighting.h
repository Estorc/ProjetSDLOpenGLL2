#ifndef LIGHTING_H
#define LIGHTING_H
void configure_global_lighting(Window *window, Node *root, Camera *c, Shader shaders[]);
void configure_directional_lighting(Window *window, Node *root, Camera *c, Shader shaders[]);
#endif