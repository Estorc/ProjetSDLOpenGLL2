/**
 * @file
 * @brief This file contains the implementation of the light class for the project.
 *
 * This source file provides the functionality for managing different types of lights
 * within the application. It includes the necessary functions to create, update, and
 * manipulate light objects, which are essential for rendering scenes with proper lighting
 * effects in the SDL and OpenGL environment.
 *
 * The light class is designed to handle various light properties such as position, color,
 * intensity, and type (e.g., directional, point, spotlights). It ensures that the lights
 * are correctly integrated into the rendering pipeline, allowing for realistic and dynamic
 * lighting in the 3D scenes.
 *
 * This file is a crucial part of the project as it directly impacts the visual quality and
 * realism of the rendered scenes by providing the necessary tools to work with lighting.
 *
 * @author Loup Picault
 * @date 2023-10-24
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/lighting.h"
#include "render/depth_map.h"
#include "buffer.h"

class Light : public Node {
    __containerType__ Node *
    public:


    void init_light() {
        this->flags |= NODE_LIGHT_ACTIVE;
    }


    static VBO vbo = 0;
    static VAO vao = 0;
    static Shader billboardShader = 0;
    static TextureMap lightPointTexture = 0;
    static TextureMap directionalLightTexture = 0;
    void render(mat4 *modelMatrix) {
        #ifdef DEBUG
        if (!vao) this::init_vao();
        use_shader(billboardShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lightPointTexture);
        
        int modelLoc = glGetUniformLocation(billboardShader, "model");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *) modelMatrix);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        #endif
    }


    void init_vao() {
        float quadVertices[] = {
            // positions        // texture Coords
            1.0f,  -1.0f,       0.0f,   1.0f,
            1.0f,   1.0f,       0.0f,   0.0f,
           -1.0f,   1.0f,       1.0f,   0.0f,

            1.0f,  -1.0f,       0.0f,   1.0f,
           -1.0f,   1.0f,       1.0f,   0.0f,
           -1.0f,  -1.0f,       1.0f,   1.0f
        };
        lightPointTexture = load_texture_from_path("assets/textures/editor/light_bulb.png", GL_SRGB_ALPHA, false);
        directionalLightTexture = load_texture_from_path("assets/textures/editor/sun.png", GL_SRGB_ALPHA, false);
        billboardShader = create_shader("shaders/billboard.vs", "shaders/billboard.fs");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    /**
     * @brief Configures the lighting for the scene.
     * 
     * This function configures the lighting for the scene by setting the light properties in the shader.
     * 
     * @param shaders The shaders to be used for rendering.
     * @param lightView The view matrix for the light.
     * @param lightProjection The projection matrix for the light.
     * @param storageBufferIndex The index of the storage buffer.
     */

    void configure_lighting(WorldShaders *shaders, mat4 *lightView, mat4 *lightProjection, int storageBufferIndex, DepthMap *depthMap) {
        mat4 lightSpaceMatrix;
        glm_mat4_mul(*lightProjection, *lightView, lightSpaceMatrix);

        if (depthMap->tbo == 0) {
            PRINT_ERROR("Error: depthMap->tbo is not initialized!");
        }        

        glBindBuffer(GL_TEXTURE_BUFFER, depthMap->tbo);

        mat4* mappedBuffer = (mat4*)glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY);
        glm_mat4_copy(lightSpaceMatrix, mappedBuffer[storageBufferIndex]);
        glUnmapBuffer(GL_TEXTURE_BUFFER);
        glActiveTexture(GL_TEXTURE9); // Use texture unit 0
        glBindTexture(GL_TEXTURE_BUFFER, depthMap->matrixTexture);
        glBindBuffer(GL_TEXTURE_BUFFER, 0);
        use_shader(shaders->render);
        set_shader_int(shaders->render, "lightMatrixBuffer", 9);
        use_shader(shaders->depth);
        set_shader_mat4(shaders->depth, "lightSpaceMatrix", &lightSpaceMatrix);
    }
}