#ifndef LIGHTING_H
#define LIGHTING_H

struct WorldShaders;
struct Window;
struct Node;
struct Camera;

/**
 * @defgroup Lighting Lighting Configuration
 * @brief Functions to manage lighting in the rendering engine.
 * @{
 */


/**
 * @enum Lightings
 * @brief Enumeration of different types of lights.
 * 
 * This enumeration defines the different types of lights that can be used in the rendering system.
 * - POINT_LIGHT: A light that emits light in all directions from a single point.
 * - DIRECTIONAL_LIGHT: A light that emits light in a specific direction, similar to sunlight.
 * - SPOT_LIGHT: A light that emits light in a cone shape, with a defined cutoff angle.
 * - LIGHTS_COUNT: The total number of light types defined.
 */

enum Lightings {
    POINT_LIGHT, /**< A point light source. */
    DIRECTIONAL_LIGHT, /**< A directional light source. */
    SPOT_LIGHT, /**< A spotlight source. */
    LIGHTS_COUNT /**< The total number of light types. */
};


/**
 * @struct PointLight
 * @brief Represents a point light source.
 * 
 * A point light emits light in all directions from a single point. This structure contains the
 * properties of a point light, including ambient, diffuse, and specular components, as well as
 * attenuation factors.
 */

typedef struct PointLight {
    vec3 ambient; /**< Ambient component of the light. */
    vec3 diffuse; /**< Diffuse component of the light. */
    vec3 specular; /**< Specular component of the light. */
    float constant; /**< Constant attenuation factor for the light. */
    float linear; /**< Linear attenuation factor for the light. */
    float quadratic; /**< Quadratic attenuation factor for the light. */
} PointLight;


/**
 * @struct DirectionalLight
 * @brief Represents a directional light source.
 * 
 * A directional light emits light in a specific direction, similar to sunlight. This structure
 * contains the properties of a directional light, including ambient, diffuse, and specular
 * components, as well as attenuation factors.
 */

typedef struct DirectionalLight {
    vec3 ambient; /**< Ambient component of the light. */
    vec3 diffuse; /**< Diffuse component of the light. */
    vec3 specular; /**< Specular component of the light. */
    float constant; /**< Constant attenuation factor for the light. */
    float linear; /**< Linear attenuation factor for the light. */
    float quadratic; /**< Quadratic attenuation factor for the light. */
} DirectionalLight;


/**
 * @struct SpotLight
 * @brief Represents a spotlight source.
 * 
 * A spotlight emits light in a cone shape, with a defined cutoff angle. This structure contains
 * the properties of a spotlight, including ambient, diffuse, and specular components, attenuation
 * factors, and cutoff angles.
 */

typedef struct SpotLight {
    vec3 ambient; /**< Ambient component of the light. */
    vec3 diffuse; /**< Diffuse component of the light. */
    vec3 specular; /**< Specular component of the light. */
    float constant; /**< Constant attenuation factor for the light. */
    float linear; /**< Linear attenuation factor for the light. */
    float quadratic; /**< Quadratic attenuation factor for the light. */
    float cutOff; /**< Cutoff angle for the spotlight. */
    float outerCutOff; /**< Outer cutoff angle for the spotlight. */
} SpotLight;


/**
 * @struct LightingBuffer
 * @brief Buffer structure to manage an array of lighting nodes.
 * 
 * This structure is used to manage an array of lighting nodes, keeping track of the length of the
 * array and the current index.
 */


typedef struct LightingBuffer {
    struct Node **lightings; /**< Array of lighting nodes. */
    u8 length; /**< Length of the array. */
    u8 index; /**< Current index in the array. */
} LightingBuffer;


/**
 * @brief Configures the global lighting settings.
 * 
 * This function sets up the global lighting parameters for the rendering engine.
 * It takes into account the window, scene graph root node, camera, and shaders
 * to configure the lighting environment.
 * 
 * @param window Pointer to the Window structure.
 * @param root Pointer to the root Node of the scene graph.
 * @param c Pointer to the Camera structure.
 * @param shaders Pointer to the WorldShaders structure containing shader programs.
 */
void configure_global_lighting(struct Window *window, struct Node *root, struct Camera *c, struct WorldShaders *shaders);

/**
 * @brief Configures the directional lighting settings.
 * 
 * This function sets up the directional lighting parameters for the rendering engine.
 * It uses the window, scene graph root node, camera, shaders, and a specific light node
 * to configure the directional light. It also takes into account the index of the light,
 * the count of lights, and the ID of the point light.
 * 
 * @param window Pointer to the Window structure.
 * @param root Pointer to the root Node of the scene graph.
 * @param c Pointer to the Camera structure.
 * @param shaders Pointer to the WorldShaders structure containing shader programs.
 * @param light Pointer to the Node representing the directional light.
 * @param index Index of the directional light.
 * @param lightsCount Array containing the count of different types of lights.
 * @param pointLightId ID of the point light.
 */
void configure_directional_lighting(struct Window *window, struct Node *root, struct Camera *c, struct WorldShaders *shaders, struct Node *light, int index, u8 lightsCount[LIGHTS_COUNT], int pointLightId);

/**
 * @brief Resets all lighting settings.
 * 
 * This function resets all lighting configurations to their default states.
 * It is useful for clearing any custom lighting settings and starting fresh.
 */
void reset_lightings();

/** @} */ // end of Lighting group
#endif