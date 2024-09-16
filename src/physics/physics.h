#include "../io/input.h"

void update_physics(struct Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer, Input *input, Window *window, bool active);
void update_global_position(struct Node *node, vec3 pos, vec3 rot, vec3 scale);