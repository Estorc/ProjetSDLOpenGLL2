#ifndef SHADER_H
#define SHADER_H


/** @brief This file contains the declarations for shader management functions.
 * 
 * The functions declared in this file are used to create, use, and set various
 * types of data in shaders. Shaders are essential for rendering graphics in 
 * OpenGL, and these functions provide a convenient interface for managing them.
 * 
 * @defgroup ShaderManagement Shader Management
 * @{
 */


/**
 * @def DEFAULT_RENDER_SHADER
 * @brief Defines the default vertex and fragment shaders for rendering shadows.
 * @details The vertex shader is located at "shaders/forwardShadowShader.vs" and the fragment shader is located at "shaders/forwardShadowShader.fs".
 */
#define DEFAULT_RENDER_SHADER "shaders/deferredShader.vs", "shaders/deferredShader.fs"

/**
 * @def DEFAULT_DEPTH_SHADER
 * @brief Defines the default vertex and fragment shaders for simple depth rendering.
 * @details The vertex shader is located at "shaders/simpleDepthShader.vs" and the fragment shader is located at "shaders/simpleDepthShader.fs".
 */
#define DEFAULT_DEPTH_SHADER "shaders/simpleDepthShader.vs", "shaders/simpleDepthShader.fs"

/**
 * @def DEFAULT_SCREEN_SHADER
 * @brief Defines the default vertex and fragment shaders for screen post-processing.
 * @details The vertex shader is located at "shaders/screen.vs" and the fragment shader is located at "shaders/screen.fs".
 */
#define DEFAULT_SCREEN_SHADER "shaders/screen.vs", "shaders/screen.fs"

/**
 * @def DEFAULT_SMAA_SHADER
 * @brief Defines the default vertex and fragment shaders for SMAA (Subpixel Morphological Anti-Aliasing).
 * @details The vertex shader is located at "shaders/smaa.vs" and the fragment shader is located at "shaders/smaa.fs".
 */
#define DEFAULT_SMAA_SHADER "shaders/smaa.vs", "shaders/smaa.fs"

/**
 * @def DEFAULT_SKYBOX_SHADER
 * @brief Defines the default vertex and fragment shaders for rendering the skybox.
 * @details The vertex shader is located at "shaders/skybox.vs" and the fragment shader is located at "shaders/skybox.fs".
 */
#define DEFAULT_SKYBOX_SHADER "shaders/skybox.vs", "shaders/skybox.fs"

/**
 * @def DEFAULT_GUI_SHADER
 * @brief Defines the default vertex and fragment shaders for rendering the GUI.
 * @details The vertex shader is located at "shaders/gui.vs" and the fragment shader is located at "shaders/gui.fs".
 */
#define DEFAULT_GUI_SHADER "shaders/gui.vs", "shaders/gui.fs"

/**
 * @def DEFAULT_SSR_SHADER
 * @brief Defines the default vertex and fragment shaders for Screen Space Reflections (SSR).
 * @details The vertex shader is located at "shaders/ssr.vs" and the fragment shader is located at "shaders/ssr.fs".
 */
#define DEFAULT_SSR_SHADER "shaders/ssr.vs", "shaders/ssr.fs"

/**
 * @def DEFAULT_LIGHT_SHADER
 * @brief Defines the default vertex and fragment shaders for rendering lights.
 * @details The vertex shader is located at "shaders/lightPass.vs" and the fragment shader is located at "shaders/lightPass.fs".
 */
#define DEFAULT_LIGHT_SHADER "shaders/lightPass.vs", "shaders/lightPass.fs"

/**
 * @def DEFAULT_SSAO_SHADER
 * @brief Defines the default vertex and fragment shaders for Screen Space Ambient Occlusion (SSAO).
 * @details The vertex shader is located at "shaders/ssao.vs" and the fragment shader is located at "shaders/ssao.fs".
 */
#define DEFAULT_SSAO_SHADER "shaders/ssao.vs", "shaders/ssao.fs"

/**
 * @def DEFAULT_SSAO_BLUR_SHADER
 * @brief Defines the default vertex and fragment shaders for SSAO blur.
 * @details The vertex shader is located at "shaders/ssaoBlur.vs" and the fragment shader is located at "shaders/ssaoBlur.fs".
 */
#define DEFAULT_SSAO_BLUR_SHADER "shaders/ssaoBlur.vs", "shaders/ssaoBlur.fs"

/**
 * @def DEFAULT_BLOOM_SHADER
 * @brief Defines the default vertex and fragment shaders for bloom effect.
 * @details The vertex shader is located at "shaders/bloom.vs" and the fragment shader is located at "shaders/bloom.fs".
 */
#define DEFAULT_BLOOM_SHADER "shaders/bloom.vs", "shaders/bloom.fs"


/**
 * @brief Creates multiple shaders.
 * 
 * This function initializes and compiles multiple shaders from predefined paths.
 * 
 * @param shaders An array of Shader objects to be created.
 */
void create_shaders(Shader shaders[]);

/**
 * @brief Creates a shader program from vertex and fragment shader files.
 * 
 * This function reads the vertex and fragment shader source code from the specified
 * file paths, compiles them, and links them into a shader program.
 * 
 * @param vertexPath The file path to the vertex shader source code.
 * @param fragmentPath The file path to the fragment shader source code.
 * @return A Shader object representing the compiled and linked shader program.
 */
Shader create_shader(char* vertexPath, char* fragmentPath);

/**
 * @brief Sets the screen size for the shader program.
 * 
 * This function sets the screen width and height for the shader program, which can be used for rendering calculations.
 * 
 * @param ID The Shader object representing the shader program.
 * @param width The width of the screen.
 * @param height The height of the screen.
 */
void set_shader_screen_size(Shader ID, int width, int height);

/**
 * @brief Sets the screen size for all shaders.
 * 
 * This function sets the screen width and height for all shaders in the shader cache.
 * 
 * @param width The width of the screen.
 * @param height The height of the screen.
 */
void set_shaders_screen_size(int width, int height);

/**
 * @brief Uses the specified shader program.
 * 
 * This function activates the shader program identified by the given Shader ID.
 * 
 * @param ID The Shader object representing the shader program to be used.
 */
void use_shader(Shader ID);

/**
 * @brief Sets an integer uniform in the shader program.
 * 
 * This function sets the value of an integer uniform variable in the shader program.
 * 
 * @param ID The Shader object representing the shader program.
 * @param name The name of the uniform variable in the shader program.
 * @param value The integer value to set for the uniform variable.
 */
void set_shader_int(Shader ID, char *name, int value);

/**
 * @brief Sets a float uniform in the shader program.
 * 
 * This function sets the value of a float uniform variable in the shader program.
 * 
 * @param ID The Shader object representing the shader program.
 * @param name The name of the uniform variable in the shader program.
 * @param value The float value to set for the uniform variable.
 */
void set_shader_float(Shader ID, char *name, float value);

/**
 * @brief Sets a vec2 uniform in the shader program.
 * 
 * This function sets the value of a vec2 (2-component vector) uniform variable in the shader program.
 * 
 * @param ID The Shader object representing the shader program.
 * @param name The name of the uniform variable in the shader program.
 * @param value The vec2 value to set for the uniform variable.
 */
void set_shader_vec2(Shader ID, char *name, vec2 value);

/**
 * @brief Sets a vec3 uniform in the shader program.
 * 
 * This function sets the value of a vec3 (3-component vector) uniform variable in the shader program.
 * 
 * @param ID The Shader object representing the shader program.
 * @param name The name of the uniform variable in the shader program.
 * @param value The vec3 value to set for the uniform variable.
 */
void set_shader_vec3(Shader ID, char *name, vec3 value);

/**
 * @brief Sets a vec4 uniform in the shader program.
 * 
 * This function sets the value of a vec4 (4-component vector) uniform variable in the shader program.
 * 
 * @param ID The Shader object representing the shader program.
 * @param name The name of the uniform variable in the shader program.
 * @param value The vec4 value to set for the uniform variable.
 */
void set_shader_vec4(Shader ID, char *name, vec4 value);

/**
 * @brief Sets a mat4 uniform in the shader program.
 * 
 * This function sets the value of a mat4 (4x4 matrix) uniform variable in the shader program.
 * 
 * @param ID The Shader object representing the shader program.
 * @param name The name of the uniform variable in the shader program.
 * @param value The mat4 value to set for the uniform variable.
 */
void set_shader_mat4(Shader ID, char *name, mat4 value);

/** @} */ // end of ShaderManagement

#endif