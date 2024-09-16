#ifndef SHADER_H
#define SHADER_H

typedef unsigned int Shader;

enum Shaders {
    SHADER_CLASSIC_LIGHTING,
    SHADER_DEPTH_DEBUG,
    SHADER_SHADOW,
    SHADER_AA,
    SHADER_SKYBOX,
    SHADER_COUNT
};

void create_shaders(Shader shaders[]);
Shader create_shader(const char* vertexPath, const char* fragmentPath);
void use_shader(Shader ID);
void set_shader_int(Shader ID, char *name, int value);
void set_shader_float(Shader ID, char *name, float value);

#endif