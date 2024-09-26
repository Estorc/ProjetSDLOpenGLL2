#include "../types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../math/math_util.h"
#include "../io/model.h"
#include "framebuffer.h"
#include "../node.h"
#include "../render/render.h"
#include "../window.h"
#include "color.h"
#include "camera.h"
#include "../memory.h"
#include "lighting.h"




void set_lightings(u8 lightsCount[LIGHTS_COUNT]) {
    for (int i = 0; i < memoryCaches.shadersCount; i++) {
        use_shader(memoryCaches.shaderCache[i].shader);
        set_shader_int(memoryCaches.shaderCache[i].shader, "pointLightsNum", lightsCount[POINT_LIGHT]);
        set_shader_int(memoryCaches.shaderCache[i].shader, "dirLightsNum", lightsCount[DIRECTIONAL_LIGHT]);
        set_shader_int(memoryCaches.shaderCache[i].shader, "spotLightsNum", lightsCount[SPOT_LIGHT]);
    }
}


/**
 * Configures the global lighting parameters for the scene.
 * 
 * @param window {Window*} Pointer to the Window structure that holds the current rendering context.
 * @param root {Node*} Pointer to the root Node structure of the scene graph.
 * @param c {Camera*} Pointer to the Camera structure that defines the view settings for rendering.
 * @param shaders {Shader[]} Array of Shader structures containing shaders used for rendering the scene.
 * 
 * This function sets the global lighting parameters for the classic lighting shader. It configures 
 * the directional light properties including position, direction, ambient, diffuse, and specular 
 * components. It also sets the properties for a point light, including position, ambient, diffuse, 
 * and specular colors, along with attenuation factors. Finally, it sets a default vertex color used 
 * in the shader. This ensures that the scene is lit correctly based on the specified light properties.
 */

void configure_global_lighting(Window *window, Node *root, Camera *c, WorldShaders *shaders) {
    use_shader(shaders->render);

    vec3 lightPos = {5.0f, 1.0f, 0.0f};
    vec3 lightDir = {c->dir[0], c->dir[1], c->dir[2]};
    glm_vec3_copy(c->pos, lightPos);

    glUniform3fv(glGetUniformLocation(shaders->render, "objectColor"), 1, (vec3){0.2f,0.2f,0.2f});

    glUniform3fv(glGetUniformLocation(shaders->render, "dirLight.position"), 1, c->pos);
    glUniform3fv(glGetUniformLocation(shaders->render, "dirLight.direction"), 1, lightDir);

    glUniform3fv(glGetUniformLocation(shaders->render, "dirLight.ambient"), 1, (vec3){0.0f,0.0f,0.0f});
    glUniform3fv(glGetUniformLocation(shaders->render, "dirLight.diffuse"), 1, (vec3){0.8f,0.8f,0.8f});
    glUniform3fv(glGetUniformLocation(shaders->render, "dirLight.specular"), 1, (vec3){0.8f,0.8f,0.8f});

    int vertexColorLocation = glGetUniformLocation(shaders->render, "ourColor");
    glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
}


/**
 * Configures the directional lighting parameters for the scene.
 * 
 * @param window {Window*} Pointer to the Window structure that holds the current rendering context.
 * @param root {Node*} Pointer to the root Node structure of the scene graph.
 * @param c {Camera*} Pointer to the Camera structure that defines the view settings for rendering.
 * @param shaders {Shader[]} Array of Shader structures containing shaders used for rendering the scene.
 * 
 * This function calculates and sets the light space matrix for directional lighting, which is 
 * used to project shadows in the scene. It creates an orthographic projection matrix and a view 
 * matrix from the light's position, combining them to form the light space matrix. This matrix 
 * is then passed to both the classic lighting shader and the shadow shader. The function ensures 
 * that shadows are cast correctly from the directional light's perspective, allowing for realistic 
 * lighting effects in the rendered scene.
 */

void configure_directional_lighting(Window *window, Node *root, Camera *c, WorldShaders *shaders) {
    // Lights and shadows
    mat4 lightProjection, lightView;
    mat4 lightSpaceMatrix;
    vec3 lightPos = {5.0f, 1.0f, 0.0f};
    glm_vec3_copy(c->pos, lightPos);

    f32 near_plane = 1.0f, far_plane = 50.0f;
    glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane, lightProjection);
    glm_lookat(lightPos, (vec3){0.0f}, (vec3){0.0, 1.0, 0.0}, lightView);

    vec3 cameraPos   = {c->pos[0],c->pos[1],c->pos[2]};
    vec3 cameraFront = {c->dir[0], c->dir[1], c->dir[2]};
    vec3 cameraUp    = {0.0f, 1.0f,  0.0f};
    vec3 cameraB;
    glm_vec3_sub(cameraPos, cameraFront, cameraB);
    glm_lookat(cameraPos, cameraB, cameraUp, lightView);


    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);

    // Cast shadow direction (render scene from light's point of view)
    use_shader(shaders->render);
    glUniformMatrix4fv(glGetUniformLocation(shaders->render, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix);
    use_shader(shaders->depth);
    glUniformMatrix4fv(glGetUniformLocation(shaders->depth, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix);
}