#ifndef LIGHTING_H
#define LIGHTING_H

enum Lightings {
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    SPOT_LIGHT,
    LIGHTS_COUNT
};

typedef struct PointLight {
    Vec3f ambient,diffuse,specular;
    float constant,linear,quadratic;
} PointLight;

typedef struct DirectionalLight {
    Vec3f ambient,diffuse,specular;
    float constant,linear,quadratic;
} DirectionalLight;

typedef struct SpotLight {
    Vec3f ambient,diffuse,specular;
    float constant,linear,quadratic;
    float cutOff, outerCutOff;
} SpotLight;

struct WorldShaders;
struct Window;
struct Node;
struct Camera;

void configure_global_lighting(struct Window *window, struct Node *root, struct Camera *c, struct WorldShaders *shaders);
void configure_directional_lighting(struct Window *window, struct Node *root, struct Camera *c, struct WorldShaders *shaders);
void reset_lightings();
#endif