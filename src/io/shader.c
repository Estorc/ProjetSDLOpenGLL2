#include "../types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "stringio.h"
#include "../math/math_util.h"
#include "shader.h"
#include "model.h"
#include "../memory.h"


/**
 * Compiles vertex and fragment shaders from specified file paths and links them into a shader program.
 *
 * @param vertexPath {const char*} The file path to the vertex shader source code.
 * @param fragmentPath {const char*} The file path to the fragment shader source code.
 *
 * @return {Shader} The identifier of the compiled and linked shader program.
 *
 * This function reads shader source code from the provided file paths, compiles 
 * both vertex and fragment shaders, and links them into a single shader program. 
 * It handles compilation errors by printing error messages to the console.
 *
 * Important Notes:
 * - Ensure that the provided file paths are valid and accessible. The function 
 *   reads the entire contents of the files as strings.
 * - The shader program ID returned by this function can be used in subsequent 
 *   rendering operations with the `use_shader` function.
 * - The function cleans up by deleting the individual shader objects after they 
 *   have been linked into the program, freeing the associated resources.
 * - If any errors occur during compilation or linking, they will be reported 
 *   to the console with the relevant error messages.
 */

Shader create_shader(char* vertexRelPath, char* fragmentRelPath) {
    char *vertexPath = relative_path(vertexRelPath);
    char *fragmentPath = relative_path(fragmentRelPath);
    for (int i = 0; i < memoryCaches.shadersCount; i++) {
        if (!strcmp(memoryCaches.shaderCache[i].shaderName[0], vertexPath) &&
            !strcmp(memoryCaches.shaderCache[i].shaderName[1], fragmentPath)) {
            #ifdef DEBUG
                printf("Shader loaded from cache!\n");
            #endif
            return memoryCaches.shaderCache[i].shader;
        }
    }
    Shader vertex, fragment;
    int success;
    char infoLog[512];

    char* vShaderCode = read_file(vertexPath);
    char* fShaderCode = read_file(fragmentPath);

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s",infoLog);
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s",infoLog);
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
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s",infoLog);
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

    free(vertexPath);
    free(fragmentPath);

    return ID;

}


/**
 * Activates a shader program for subsequent rendering operations.
 *
 * @param ID {Shader} The identifier of the shader program to be used. This 
 *                    should be the program ID obtained after linking the shader 
 *                    program.
 *
 * This function sets the specified shader program as the current active program 
 * in OpenGL, allowing the subsequent rendering commands to utilize the shaders 
 * associated with the specified program.
 *
 * Important Notes:
 * - It is crucial to call this function before issuing any rendering commands 
 *   (such as drawing vertices or setting uniforms) that rely on the shader program.
 * - Ensure that the shader program has been compiled and linked successfully 
 *   before calling this function. If the shader program is not valid, rendering 
 *   will not behave as expected.
 * - If you switch between multiple shader programs during rendering, ensure to 
 *   call `use_shader` with the appropriate program ID before rendering each object 
 *   that requires a different shader.
 */

void use_shader(Shader ID) {
    glUseProgram(ID);
}


/**
 * Sets an integer uniform variable in a shader program.
 *
 * @param ID {Shader} The identifier of the shader program where the uniform 
 *                    variable is located. This should be the program ID 
 *                    obtained after linking the shader program.
 * @param name {char *} A pointer to a null-terminated string representing 
 *                      the name of the uniform variable in the shader 
 *                      program. This must match the variable name exactly 
 *                      as defined in the shader source code.
 * @param value {int} The integer value to be assigned to the uniform variable.
 * 
 * This function retrieves the location of the specified integer uniform variable 
 * in the shader program identified by the given ID and sets its value. It 
 * should be called after the shader program has been linked and before the 
 * rendering process begins.
 *
 * Important Notes:
 * - The shader program must be in use (i.e., `glUseProgram(ID)` should be called) 
 *   before calling this function to ensure that the uniform variable is set in 
 *   the correct shader context.
 * - Ensure that the name of the uniform variable matches the name defined in 
 *   the shader code, otherwise the value may not be updated as expected.
 * - It is advisable to check if the uniform location returned by 
 *   `glGetUniformLocation` is valid (i.e., not -1) before attempting to set its value. 
 *   If the location is -1, it indicates that the uniform variable is not found, 
 *   which may be due to a typo in the name or because the variable is optimized 
 *   out by the shader compiler.
 */

void set_shader_int(Shader ID, char *name, int value) { 
    glUniform1i(glGetUniformLocation(ID, name), value); 
}


/**
 * Sets a float uniform variable in a shader program.
 *
 * @param ID {Shader} The identifier of the shader program where the uniform 
 *                    variable is located. This should be the program ID 
 *                    obtained after linking the shader program.
 * @param name {char *} A pointer to a null-terminated string representing 
 *                      the name of the uniform variable in the shader 
 *                      program. This must match the variable name exactly 
 *                      as defined in the shader source code.
 * @param value {float} The float value to be assigned to the uniform variable.
 * 
 * This function retrieves the location of the specified float uniform variable 
 * in the shader program identified by the given ID and sets its value. It 
 * should be called after the shader program has been linked and before the 
 * rendering process begins.
 *
 * Important Notes:
 * - The shader program must be in use (i.e., `glUseProgram(ID)` should be called) 
 *   before calling this function to ensure that the uniform variable is set in 
 *   the correct shader context.
 * - Ensure that the name of the uniform variable matches the name defined in 
 *   the shader code, otherwise the value may not be updated as expected.
 * - It is advisable to check if the uniform location returned by 
 *   `glGetUniformLocation` is valid (i.e., not -1) before attempting to set its value. 
 *   If the location is -1, it indicates that the uniform variable is not found, 
 *   which may be due to a typo in the name or because the variable is optimized 
 *   out by the shader compiler.
 */

void set_shader_float(Shader ID, char *name, float value) { 
    glUniform1f(glGetUniformLocation(ID, name), 1, &value);
}

void set_shader_vec3(Shader ID, char *name, vec3 value) {
    glUniform3fv(glGetUniformLocation(ID, name), 1, value);
}