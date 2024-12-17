#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H
struct Node;
struct Camera;

struct Node *load_node(FILE *file, struct Camera **c, Script scripts[SCRIPTS_COUNT], struct Node *editor);
struct Node *load_scene(char *path, struct Camera **c, Script scripts[SCRIPTS_COUNT]);
#endif