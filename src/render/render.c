#include <raptiquax.h>
#include <math/math_util.h>
#include <io/model.h>
#include <framebuffer.h>
#include <storage/node.h>
#include <depth_map.h>
#include <render.h>
#include <window.h>
#include <camera.h>
#include <io/gltexture_loader.h>
#include <lighting.h>
#include <buffer.h>
#include <classes/classes.h>
#include <settings.h>


void render_scene(Window *window, Node *node, Camera *c, mat4 modelMatrix, Shader activeShader, WorldShaders *shaders, int viewportWidth, int viewportHeight) {
    if (node->flags & NODE_VISIBLE) {
        FBO currentFBO = get_fbo();
        bool is_render_target = false;
        bool is_gui_element = false;
        node::is_render_target(&is_render_target);
        node::is_gui_element(&is_gui_element);
        RenderTarget *lastRenderTarget = Game.renderTarget;

        if (is_render_target) {
            RenderTarget *renderTarget = (RenderTarget *) node->object;
            Game.renderTarget = renderTarget;
            use_fbo(GL_FRAMEBUFFER, renderTarget->fbo);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, renderTarget->w, renderTarget->h); // Make sure viewport matches FBO size
        }

        if (!Game.renderTarget) {
            if (is_gui_element && currentFBO != Game.uiFBO->framebuffer) {
                use_fbo(GL_FRAMEBUFFER, Game.uiFBO->framebuffer);
            }
        }
        
        mat4 nodeModelMatrix = GLM_MAT4_IDENTITY_INIT;
        glm_mat4_copy(modelMatrix, nodeModelMatrix);

        node::prepare_render(nodeModelMatrix, activeShader, shaders);
        node::render(nodeModelMatrix, activeShader, shaders);

        for (int i = 0; i < node->length; i++) {
            render_scene(window, node->children[i], c, nodeModelMatrix, activeShader, shaders, viewportWidth, viewportHeight);
        }

        #ifdef DEBUG
        if (Game.settings->show_collision_boxes) {
            bool is_body = false;
            node::is_body((&is_body));
            if (!is_body) {
                node::is_area(&is_body);
            }
            if (is_body) {
                u8 *length;
                Node ***shapes;
                node::get_collisions_shapes(&shapes, &length);
                for (int i = 0; i < *length; i++) {
                    render_scene(window, (*shapes)[i], c, nodeModelMatrix, activeShader, shaders, viewportWidth, viewportHeight);
                }
            }
        }
        #endif

        if (!Game.renderTarget) {
            if (is_gui_element) {
                use_fbo(GL_FRAMEBUFFER, currentFBO);
            }
        }

        if (is_render_target) {
            Game.renderTarget = lastRenderTarget;
            use_fbo(GL_FRAMEBUFFER, currentFBO);
            glViewport(0, 0, viewportWidth, viewportHeight);
        }
    }

}

void draw_shadow_map(Window *window, Node *root, Camera *c, WorldShaders *shaders, DepthMap *depthMap) {
    // Draw shadow map (render scene with depth map shader)
    if (!Game.settings->cast_shadows) return;
    glCullFace(GL_FRONT);
    glViewport(0, 0, Game.settings->shadow_resolution, Game.settings->shadow_resolution);
    u8 lightsCount[LIGHTS_COUNT] = {0};
    mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
    use_shader(shaders->depth);
    for (int i = 0, index = 0, pl = 0; i < Game.buffers->lightingBuffer.index; i++, index++) {
        use_fbo(GL_FRAMEBUFFER, depthMap->frameBuffer[index]);
        (Game.buffers->lightingBuffer.lightings[i])::configure_lighting(c, shaders, depthMap, lightsCount, pl);
        const GLfloat clearDepth = 1.0f;
        glClearBufferfv(GL_DEPTH, 0, &clearDepth);
        render_scene(window, root, c, modelMatrix, shaders->depth, shaders, Game.settings->shadow_resolution, Game.settings->shadow_resolution);
        if (Game.buffers->lightingBuffer.lightings[i]->type == CLASS_TYPE_POINTLIGHT && pl < 5) {
            i--;
            pl++;
        } else pl = 0;
    }
    glCullFace(GL_BACK);
    use_fbo(GL_FRAMEBUFFER, 0);
}


void draw_scene(Window *window, Node *root, Camera *c, WorldShaders *shaders, DepthMap *depthMap) {

    // Draw scene (render scene with scene shader)

    int window_width, window_height;
    get_resolution(&window_width, &window_height);

    glViewport(0, 0, window_width, window_height);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    use_shader(shaders->light);
    set_shader_int(shaders->light, "shadowCastActive", Game.settings->cast_shadows);
    set_shader_int(shaders->light, "shadowQuality", Game.settings->shadow_quality);

    use_shader(shaders->render);
    set_shader_int(shaders->render, "diffuseMap", 0);
    set_shader_int(shaders->render, "normalMap", 1);
    set_shader_int(shaders->render, "parallaxMap", 2);
    set_shader_int(shaders->render, "metallicMap", 4);
    set_shader_int(shaders->render, "roughnessMap", 5);

    mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
    render_scene(window, root, c, modelMatrix, shaders->render, shaders, window_width, window_height);
}

void draw_screen(Window *window, Node *scene, Camera *c, WorldShaders *shaders, DepthMap *depthMap, Mesh *screenPlane) {

    int window_width, window_height;
    get_resolution(&window_width, &window_height);

    glEnable(GL_FRAMEBUFFER_SRGB);  // Enable sRGB framebuffer correction
    glEnable(GL_MULTISAMPLE);  
    glEnable(GL_DEPTH_TEST);

    camera_projection(c,shaders);
    draw_shadow_map(window,scene,c,shaders,depthMap);
    
    use_fbo(GL_FRAMEBUFFER, Game.uiFBO->framebuffer);
    glClear(GL_COLOR_BUFFER_BIT);
    use_fbo(GL_FRAMEBUFFER, Game.deferredBuffer->framebuffer);
    draw_scene(window,scene,c,shaders,depthMap);
    glBindVertexArray(screenPlane->VAO);

    glViewport(0, 0, window_width, window_height);
    if (Game.settings->ssao) {
        use_intermediate_fbo();
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            use_shader(shaders->ssao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->texturePositionBuffer);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->textureNormalBuffer);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->textureExtraComponent);
        glDrawArrays(GL_TRIANGLES, 0, screenPlane->length);

        use_intermediate_fbo();
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            use_shader(shaders->ssaoBlur);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, get_intermediate_texture());
        glDrawArrays(GL_TRIANGLES, 0, screenPlane->length);
    }

    use_intermediate_fbo();
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        use_shader(shaders->light);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->textureColorBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->texturePositionBuffer);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->textureNormalBuffer);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->textureExtraComponent);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, get_intermediate_texture());
        set_shader_int(shaders->light, "ColorBuffer", 0);
        set_shader_int(shaders->light, "gPosition", 1);
        set_shader_int(shaders->light, "gNormal", 2);
        set_shader_int(shaders->light, "gExtraComponents", 3);
        set_shader_int(shaders->light, "ssao", 4);
        set_shader_int(shaders->light, "ssaoActive", Game.settings->ssao);
    glDrawArrays(GL_TRIANGLES, 0, screenPlane->length);

    if (Game.settings->ssr) {
        use_intermediate_fbo();
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            use_shader(shaders->ssr);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->textureColorBuffer);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->texturePositionBuffer);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->textureNormalBuffer);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, Game.deferredBuffer->textureExtraComponent);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, get_intermediate_texture());
            set_shader_int(shaders->ssr, "ColorBuffer", 0);
            set_shader_int(shaders->ssr, "gPosition", 1);
            set_shader_int(shaders->ssr, "gNormal", 2);
            set_shader_int(shaders->ssr, "gExtraComponents", 3);
            set_shader_int(shaders->ssr, "gFinalImage", 4);
        glDrawArrays(GL_TRIANGLES, 0, screenPlane->length);
    }

    if (Game.settings->antialiasing) {
        use_intermediate_fbo();
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            use_shader(shaders->smaa);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, get_intermediate_texture());
            set_shader_int(shaders->smaa, "gFinalImage", 0);
        glDrawArrays(GL_TRIANGLES, 0, screenPlane->length);
    }

    if (Game.settings->bloom) {
        use_intermediate_fbo();
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            use_shader(shaders->bloom);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, get_intermediate_texture());
            set_shader_int(shaders->bloom, "gFinalImage", 0);
        glDrawArrays(GL_TRIANGLES, 0, screenPlane->length);
    }

    swap_intermediate_fbo();
    SDL_GetWindowSize(Game.window->sdl_window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);
    use_fbo(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        use_shader(shaders->screen);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, get_intermediate_texture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Game.uiFBO->texture);
        set_shader_int(shaders->screen, "screenTexture", 0);
        set_shader_int(shaders->screen, "UITexture", 1);
        set_shader_vec4(shaders->screen, "fadeColor", window->fadeColor);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glDrawArrays(GL_TRIANGLES, 0, screenPlane->length);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glBindVertexArray(0);


}