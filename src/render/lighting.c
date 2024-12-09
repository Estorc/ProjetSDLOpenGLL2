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
#include "render.h"
#include "../memory.h"
#include "lighting.h"
#include "depth_map.h"
#include "../classes/classes.h"




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

    //vec3 lightPos = {5.0f, 1.0f, 0.0f};
    //glm_vec3_copy(c->pos, lightPos);

    glUniform3fv(glGetUniformLocation(shaders->render, "objectColor"), 1, (vec3){0.2f,0.2f,0.2f});

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

void configure_directional_lighting(Window *window, Node *root, Camera *c, WorldShaders *shaders, Node *light, int index, u8 lightsCount[LIGHTS_COUNT], int pointLightId) {

    // Lights and shadows
    mat4 lightProjection, lightView;
    mat4 lightSpaceMatrix;

    size_t storageBufferIndex;

    switch (light->type) {
        case CLASS_TYPE_POINTLIGHT:
            {
            f32 near_plane = 1.0f, far_plane = 50.0f;
            glm_perspective(to_radians(90.0f), SHADOW_WIDTH/SHADOW_HEIGHT, near_plane, far_plane, lightProjection);
            //glm_ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane, lightProjection);

            vec3 directions[6] = {
                { 1.0f, 0.0f, 0.0f},
                {-1.0f, 0.0f, 0.0f},
                { 0.0f, 1.0f, 0.0f},
                { 0.0f,-1.0f, 0.0f},
                { 0.0f, 0.0f, 1.0f},
                { 0.0f, 0.0f,-1.0f},
            };

            vec3 lightPos   = {light->globalPos[0], light->globalPos[1], light->globalPos[2]};
            vec3 lightUp    = {0.0f, 1.0f,  0.0f};
            vec3 lightB;
            glm_vec3_sub(lightPos, directions[pointLightId], lightB);
            glm_lookat(lightPos, lightB, lightUp, lightView);

            storageBufferIndex = (lightsCount[POINT_LIGHT]*6+pointLightId)*sizeof(mat4)+100*sizeof(mat4);
            if (pointLightId == 5) lightsCount[POINT_LIGHT]++;
            }
            break;
        case CLASS_TYPE_DIRECTIONALLIGHT:
            {
            f32 near_plane = -50.0f, far_plane = 50.0f;
            glm_ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane, lightProjection);
            vec3 dir = {1.0, 0.0, 0.0};

            glm_vec3_rotate(dir, to_radians(light->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
            glm_vec3_rotate(dir, to_radians(light->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
            glm_vec3_rotate(dir, to_radians(light->rot[2]), (vec3){0.0f, 0.0f, 1.0f});

            vec3 lightPos   = {c->pos[0], c->pos[1], c->pos[2]};
            vec3 lightFront = {dir[0], dir[1], dir[2]};
            vec3 lightUp    = {0.0f, 1.0f,  0.0f};
            vec3 lightB;
            glm_vec3_sub(lightPos, lightFront, lightB);
            glm_lookat(lightPos, lightB, lightUp, lightView);

            storageBufferIndex = lightsCount[DIRECTIONAL_LIGHT]*sizeof(mat4)+0*sizeof(mat4);
            }
            lightsCount[DIRECTIONAL_LIGHT]++;
            break;
        case CLASS_TYPE_SPOTLIGHT:
            {
            f32 near_plane = 1.0f, far_plane = 50.0f;
            glm_perspective(to_radians(90.0f), SHADOW_WIDTH/SHADOW_HEIGHT, near_plane, far_plane, lightProjection);
            //glm_ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane, lightProjection);
            vec3 dir;

            dir[0] = sin(-light->globalRot[1] * PI/180);
            dir[1] = light->globalRot[0] * PI/180;
            dir[2] = -cos(-light->globalRot[1] * PI/180);

            vec3 lightPos   = {light->globalPos[0], light->globalPos[1], light->globalPos[2]};
            vec3 lightFront = {dir[0], dir[1], dir[2]};
            vec3 lightUp    = {0.0f, 1.0f,  0.0f};
            vec3 lightB;
            glm_vec3_sub(lightPos, lightFront, lightB);
            glm_lookat(lightPos, lightB, lightUp, lightView);

            storageBufferIndex = lightsCount[SPOT_LIGHT]*sizeof(mat4)+200*sizeof(mat4);
            }
            lightsCount[SPOT_LIGHT]++;
            break;
    }


    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);

    // Cast shadow direction (render scene from light's point of view)
    use_shader(shaders->render);
    glBufferSubData(GL_UNIFORM_BUFFER, storageBufferIndex, sizeof(mat4), &lightSpaceMatrix);
    use_shader(shaders->depth);
    glUniformMatrix4fv(glGetUniformLocation(shaders->depth, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix);
}