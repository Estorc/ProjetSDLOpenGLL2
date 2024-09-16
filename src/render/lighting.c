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
#include "../window.h"
#include "color.h"
#include "camera.h"


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

void configure_global_lighting(Window *window, Node *root, Camera *c, Shader shaders[]) {
    use_shader(shaders[SHADER_CLASSIC_LIGHTING]);

    vec3 lightPos = {5.0f, 1.0f, 0.0f};
    vec3 lightDir;
    glm_vec3_copy(c->pos, lightPos);
    glm_vec3_copy(c->dir, lightDir);

    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "objectColor"), 1, (vec3){0.2f,0.2f,0.2f});

    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "dirLight.position"), 1, c->pos);
    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "dirLight.direction"), 1, c->dir);

    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "dirLight.ambient"), 1, (vec3){0.0f,0.0f,0.0f});
    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "dirLight.diffuse"), 1, (vec3){0.8f,0.8f,0.8f});
    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "dirLight.specular"), 1, (vec3){0.8f,0.8f,0.8f});

    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "pointLights[0].ambient"), 1, (vec3){0.0f,0.0f,0.0f});
    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "pointLights[0].diffuse"), 1, (vec3){2.0f,2.0f,2.0f});
    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "pointLights[0].specular"), 1, (vec3){2.0f,2.0f,2.0f});

    glUniform3fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "lightPos"), 1, lightPos);

    float lightConstant = 1.0f;
    glUniform1f(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "pointLights[0].constant"), 1, &lightConstant);
    float lightLinear = 0.09f;
    glUniform1f(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "pointLights[0].linear"), 1, &lightLinear);
    float lightQuadratic = 0.032f;
    glUniform1f(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "pointLights[0].quadratic"), 1, &lightQuadratic);

    int vertexColorLocation = glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "ourColor");
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

void configure_directional_lighting(Window *window, Node *root, Camera *c, Shader shaders[]) {
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
    use_shader(shaders[SHADER_CLASSIC_LIGHTING]);
    glUniformMatrix4fv(glGetUniformLocation(shaders[SHADER_CLASSIC_LIGHTING], "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix);
    use_shader(shaders[SHADER_SHADOW]);
    glUniformMatrix4fv(glGetUniformLocation(shaders[SHADER_SHADOW], "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix);
}