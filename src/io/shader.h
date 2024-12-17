#ifndef SHADER_H
#define SHADER_H

typedef unsigned int Shader;

#define DEFAULT_RENDER_SHADER "shaders/shadowShader.vs", "shaders/shadowShader.fs"
#define DEFAULT_DEPTH_SHADER "shaders/simpleDepthShader.vs", "shaders/simpleDepthShader.fs"
#define DEFAULT_SCREEN_SHADER "shaders/aa_post.vs", "shaders/aa_post.fs"
#define DEFAULT_SKYBOX_SHADER "shaders/skybox.vs", "shaders/skybox.fs"
#define DEFAULT_GUI_SHADER "shaders/gui.vs", "shaders/gui.fs"

void create_shaders(Shader shaders[]);
Shader create_shader(char* vertexPath, char* fragmentPath);
void use_shader(Shader ID);
void set_shader_int(Shader ID, char *name, int value);
void set_shader_float(Shader ID, char *name, float value);
void set_shader_vec2(Shader ID, char *name, vec2 value);
void set_shader_vec3(Shader ID, char *name, vec3 value);
void set_shader_vec4(Shader ID, char *name, vec4 value);
void set_shader_mat4(Shader ID, char *name, mat4 *value);

#endif