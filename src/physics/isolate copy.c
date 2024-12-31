#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../window.h"
#include "../render/color.h"
#include "../render/camera.h"
#include "../render/depth_map.h"
#include "../render/lighting.h"
#include "../io/gltexture_loader.h"
#include "../classes/classes.h"
#include "../memory.h"
#include "../buffer.h"
#include "physics.h"
#include "bodies.h"
#include "collision.h"
#include "collision_util.h"


bool check_collision_sphere_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_plane_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_capsule_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_mesh_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_ray_with_ray(struct Node *shapeA, struct Node *shapeB);