#include "../raptiquax.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "framebuffer.h"
#include "../storage/node.h"
#include "depth_map.h"
#include "render.h"
#include "../window.h"
#include "camera.h"
#include "../io/gltexture_loader.h"
#include "lighting.h"
#include "../buffer.h"
#include "../classes/classes.h"
#include "../settings.h"


void render_scene(Window *window, Node *node, Camera *c, mat4 modelMatrix, Shader activeShader, WorldShaders *shaders) {

    if (node->flags & NODE_VISIBLE) {
        mat4 nodeModelMatrix = GLM_MAT4_IDENTITY_INIT;
        glm_mat4_mul(nodeModelMatrix, modelMatrix, nodeModelMatrix);
        use_shader(activeShader);
        node::prepare_render(nodeModelMatrix, activeShader, shaders);
        node::render(nodeModelMatrix, activeShader, shaders);
        for (int i = 0; i < node->length; i++) {
            render_scene(window, node->children[i], c, nodeModelMatrix, activeShader, shaders);
        }
        if (settings.show_collision_boxes) {
            bool is_body = false;
            node::is_body((&is_body));
            if (is_body) {
                u8 *length;
                Node ***shapes;
                node::get_collisions_shapes(&shapes, &length);
                for (int i = 0; i < *length; i++) {
                    render_scene(window, (*shapes)[i], c, nodeModelMatrix, activeShader, shaders);
                }
            }
        }
    }

}

void draw_shadow_map(Window *window, Node *root, Camera *c, WorldShaders *shaders, DepthMap *depthMap) {
    // Draw shadow map (render scene with depth map shader)
    if (!settings.cast_shadows) return;
    glCullFace(GL_FRONT);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMap->frameBuffer);
    u8 lightsCount[LIGHTS_COUNT] = {0};
    for (int i = 0, index = 0, pl = 0; i < buffers.lightingBuffer.index; i++, index++) {
        (buffers.lightingBuffer.lightings[i])::configure_lighting(c,shaders, lightsCount, pl);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap->texture, 0, index);
        glClear(GL_DEPTH_BUFFER_BIT);
        mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
        render_scene(window, root, c, modelMatrix, shaders->depth, shaders);
        if (buffers.lightingBuffer.lightings[i]->type == CLASS_TYPE_POINTLIGHT && pl < 5) {
            i--;
            pl++;
        } else pl = 0;
    }
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void draw_scene(Window *window, Node *root, Camera *c, WorldShaders *shaders, DepthMap *depthMap) {

    // Draw scene (render scene with scene shader)

    int window_width, window_height;
    get_resolution(&window_width, &window_height);

    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    use_shader(shaders->render);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap->texture);
    set_shader_int(shaders->render, "diffuseMap", 0);
    set_shader_int(shaders->render, "normalMap", 1);
    set_shader_int(shaders->render, "parallaxMap", 2);
    set_shader_int(shaders->render, "shadowMap", 3);
    set_shader_int(shaders->render, "shadowCastActive", settings.cast_shadows);

    mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
    render_scene(window, root, c, modelMatrix, shaders->render, shaders);
}

void draw_screen(Window *window, Node *scene, Camera *c, WorldShaders *shaders, DepthMap *depthMap, MSAA *msaa, Mesh *screenPlane) {

    int window_width, window_height;
    get_resolution(&window_width, &window_height);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_FRAMEBUFFER_SRGB); 
    glEnable(GL_MULTISAMPLE);  
    glEnable(GL_DEPTH_TEST);

    camera_projection(c,shaders);
    draw_shadow_map(window,scene,c,shaders,depthMap);
    

    glBindFramebuffer(GL_FRAMEBUFFER, msaa->framebuffer);
    draw_scene(window,scene,c,shaders,depthMap);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa->framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msaa->intermediateFBO);
    glBlitFramebuffer(0, 0, window_width, window_height, 0, 0, window_width, window_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Draw screen (render scene in the screen plane)
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    use_shader(shaders->screen);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, msaa->screenTexture); // use the now resolved color attachment as the quad's texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, window->ui_texture); // use the now resolved color attachment as the quad's texture
    set_shader_int(shaders->screen, "screenTexture", 0);
    set_shader_int(shaders->screen, "UITexture", 1);


    SDL_GetWindowSize(window->sdl_window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);
    
    glBindVertexArray(screenPlane->VAO);
    glDrawArrays(GL_TRIANGLES, 0, screenPlane->length);
    glBindVertexArray(0);
}