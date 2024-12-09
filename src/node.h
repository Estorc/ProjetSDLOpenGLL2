#include "io/shader.h"
#include "render/filter.h"
#include "render/viewport.h"
#include "render/camera.h"
#include "physics/bodies.h"
#include "scripts/scripts.h"

#ifndef NODE_H
#define NODE_H

typedef struct Node {
    void *object;
    struct Node **children;
    struct Node *parent;
    u16 length;
    u8 type;
    u8 flags;

    Vec3f pos;
    Vec3f rot;
    Vec3f scale;

    Vec3f globalPos;
    Vec3f globalRot;
    Vec3f globalScale;

    NODE_FUNC_RETURN (*script)(NODE_FUNC_PARAMS);
    ScriptParameter *params;
    u8 params_count;

    Shader shader;
} Node;

enum NodeType {
    NODE_EMPTY,
    NODE_MODEL,
    NODE_TEXTURED_MESH,
    NODE_SKYBOX,
    NODE_MESH,
    NODE_FRAMEBUFFER,
    NODE_TEXTURE,
    NODE_VIEWPORT,
    NODE_FILTER,

    _M_NODE_BODIES__START__,

    NODE_STATIC_BODY,
    NODE_RIGID_BODY,
    NODE_KINEMATIC_BODY,

    _M_NODE_BODIES__END__,


    _M_NODE_CSHAPES__START__,

    NODE_BOX_CSHAPE,
    NODE_SPHERE_CSHAPE,
    NODE_PLANE_CSHAPE,

    _M_NODE_CSHAPES__END__,

    NODE_CAMERA,
    NODE_POINT_LIGHT,
    NODE_DIRECTIONAL_LIGHT,
    NODE_SPOT_LIGHT,
    NODE_COUNT,
};


enum NodeFlags {
    NODE_ACTIVE             = 1 << 0, // 0000 0001
    NODE_VISIBLE            = 1 << 1, // 0000 0010
    NODE_SCRIPT             = 1 << 2, // 0000 0100
    NODE_UNUSED2            = 1 << 3, // 0000 1000
    NODE_UNUSED3            = 1 << 4, // 0001 0000
    NODE_UNUSED4            = 1 << 5, // 0010 0000
    NODE_UNUSED5            = 1 << 6, // 0100 0000
    NODE_EDITOR_FLAG        = 1 << 7, // 1000 0000
};

#define NODE_ACTIVE_AND_VISIBLE NODE_ACTIVE | NODE_VISIBLE // 0000 0011
#define NODE_DEFAULT_FLAGS NODE_ACTIVE_AND_VISIBLE // 0000 0011

#endif

struct PointLight;
struct DirectionalLight;
struct SpotLight;
struct Camera;
struct StaticBody;
struct RigidBody;
struct KinematicBody;
struct PlaneCollisionShape;
struct BoxCollisionShape;
struct SphereCollisionShape;
struct Model;
struct TexturedMesh;
struct Mesh;
struct Viewport;
struct Filter;

void add_child(Node *node, Node *child);
void add_child_and_realloc(Node *node, Node *child);
void remove_child(Node *node, Node *child);
void remove_child_and_realloc(Node *node, Node *child);
void remove_child_and_free(Node *node, Node *child);
void remove_child_and_free_and_realloc(Node *node, Node *child);

void render_node(Node *node, mat4 modelMatrix);
void render_point_light(Node *node, mat4 modelMatrix);
void render_directional_light(Node *node, mat4 modelMatrix);

void free_node(Node *node);
void print_node(Node *node, int level);