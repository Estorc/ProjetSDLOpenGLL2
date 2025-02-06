#include "../raptiquax.h"
#include "../math/math_util.h"
#include "shader.h"
#include "model.h"
#include "../memory.h"

Shader create_shader(char* vertexPath, char* fragmentPath) {
    for (int i = 0; i < memoryCaches.shadersCount; i++) {
        if (!strcmp(memoryCaches.shaderCache[i].shaderName[0], vertexPath) &&
            !strcmp(memoryCaches.shaderCache[i].shaderName[1], fragmentPath)) {
            #ifdef DEBUG
                PRINT_INFO("Shader loaded from cache!\n");
            #endif
            return memoryCaches.shaderCache[i].shader;
        }
    }

    #ifdef DEBUG
        PRINT_INFO("Compiling shader: %s, %s\n", vertexPath, fragmentPath);
    #endif

    Shader vertex, fragment;
    int success;
    char infoLog[512];

    char* vShaderCode = read_file(vertexPath);
    char* fShaderCode = read_file(fragmentPath);

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar * const*) &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        PRINT_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s",infoLog);
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar * const*) &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        PRINT_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s",infoLog);
    };

    Shader ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // affiche les erreurs d'édition de liens si besoin
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        PRINT_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s",infoLog);
    }
    
    // supprime les shaders qui sont maintenant liés dans le programme et qui ne sont plus nécessaires
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vShaderCode);
    free(fShaderCode);

    memoryCaches.shaderCache = realloc(memoryCaches.shaderCache, sizeof (ShaderCache) * (++memoryCaches.shadersCount));
    memoryCaches.shaderCache[memoryCaches.shadersCount-1].shader = ID;
    strcpy(memoryCaches.shaderCache[memoryCaches.shadersCount-1].shaderName[0], vertexPath);
    strcpy(memoryCaches.shaderCache[memoryCaches.shadersCount-1].shaderName[1], fragmentPath);

    return ID;

}


void use_shader(Shader ID) {
    glUseProgram(ID);
}

void set_shader_int(Shader ID, char *name, int value) { 
    glUniform1i(glGetUniformLocation(ID, name), value); 
}

void set_shader_float(Shader ID, char *name, f32 value) { 
    glUniform1fv(glGetUniformLocation(ID, name), 1, &value);
}

void set_shader_vec2(Shader ID, char *name, vec2 value) {
    glUniform2fv(glGetUniformLocation(ID, name), 1, value);
}

void set_shader_vec3(Shader ID, char *name, vec3 value) {
    glUniform3fv(glGetUniformLocation(ID, name), 1, value);
}

void set_shader_vec4(Shader ID, char *name, vec4 value) {
    glUniform4fv(glGetUniformLocation(ID, name), 1, value);
}

void set_shader_mat4(Shader ID, char *name, mat4 *value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, (const GLfloat *) value);
}