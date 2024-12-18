#include "../io/input.h"

struct Input;
struct Window;

float get_velocity_norm(struct Node *node);
void get_velocity(struct Node *node, vec3 velocity);
void get_mass(struct Node *node, float *mass);
void get_center_of_mass(struct Node *node, vec3 com);
void apply_collision(struct Node *shapeA, struct Node *shapeB, vec3 collisionNormal, vec3 angularNormal, float penetrationDepth);
void check_collisions(struct Node *shape);
void update_script(struct Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, struct Input *input, struct Window *window);
void update_physics(struct Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, struct Input *input, struct Window *window, u8 lightsCount[LIGHTS_COUNT], bool active);
void update_global_position(struct Node *node, vec3 pos, vec3 rot, vec3 scale);