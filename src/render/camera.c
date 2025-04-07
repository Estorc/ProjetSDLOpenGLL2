#include <raptiquax.h>
#include <render/camera.h>
#include <render/render.h>
#include <math/math_util.h>
#include <io/model.h>
#include <io/shader.h>
#include <memory.h>
#include <storage/node.h>
#include <window.h>
#include <settings.h>


void init_camera(Camera *c) {

    glm_vec3_zero(c->pos);
    glm_vec3_zero(c->rot);
    c->fov = Game.settings->fov;

}

void camera_projection(Camera *c, WorldShaders *shaders) {
    int window_width, window_height;
    get_resolution(&window_width, &window_height);
    // Camera
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    mat4 invView;
    mat4 invProjection;

    glm_rotate(view, to_radians(c->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(view, to_radians(c->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(view, to_radians(c->rot[2]), (vec3){0.0f, 0.0f, 1.0f});
    glm_translate(view, (vec3){c->pos[0], c->pos[1], c->pos[2]});

    glm_perspective(c->fov, (float)window_width/(float)window_height, 0.1f, 300.0f, projection);

    glm_mat4_inv_fast(view, invView);
    glm_mat4_inv_fast(projection, invProjection);

    vec3 viewPos;
    glm_vec3_negate_to(c->pos, viewPos);

    for (int i = 0; i < Game.memoryCaches->shadersCount; i++) {
        use_shader(Game.memoryCaches->shaderCache[i].shader);
        glUniformMatrix4fv(glGetUniformLocation(Game.memoryCaches->shaderCache[i].shader, "projection"), 1, GL_FALSE, (const GLfloat *) &projection);
        glUniformMatrix4fv(glGetUniformLocation(Game.memoryCaches->shaderCache[i].shader, "view"), 1, GL_FALSE, (const GLfloat *) &view);
        glUniformMatrix4fv(glGetUniformLocation(Game.memoryCaches->shaderCache[i].shader, "invProjection"), 1, GL_FALSE, (const GLfloat *) &invProjection);
        glUniformMatrix4fv(glGetUniformLocation(Game.memoryCaches->shaderCache[i].shader, "invView"), 1, GL_FALSE, (const GLfloat *) &invView);
        glUniform3fv(glGetUniformLocation(Game.memoryCaches->shaderCache[i].shader, "viewPos"), 1, (const GLfloat *) &viewPos);
    }
}