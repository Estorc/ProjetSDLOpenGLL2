#include "../io/input.h"

struct Input;
struct Window;

void update_physics(struct Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, struct Input *input, struct Window *window, u8 lightsCount[LIGHTS_COUNT], bool active);
void update_global_position(struct Node *node, vec3 pos, vec3 rot, vec3 scale);