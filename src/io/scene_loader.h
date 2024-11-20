#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H
Node *load_node(FILE *file, Camera **c, Script scripts[SCRIPTS_COUNT], Node *editor);
Node *load_scene(char *path, Camera **c, Script scripts[SCRIPTS_COUNT]);
#endif