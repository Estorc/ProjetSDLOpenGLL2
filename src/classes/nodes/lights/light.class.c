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

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

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

    void configure_lighting(WorldShaders *shaders, mat4 *lightView, mat4 *lightProjection, int storageBufferIndex) {
        mat4 lightSpaceMatrix;
        glm_mat4_mul(*lightProjection, *lightView, lightSpaceMatrix);

        // Cast shadow direction (render scene from light's point of view)
        use_shader(shaders->render);
        glBufferSubData(GL_UNIFORM_BUFFER, storageBufferIndex, sizeof(mat4), &lightSpaceMatrix);
        use_shader(shaders->depth);
        glUniformMatrix4fv(glGetUniformLocation(shaders->depth, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix);
    }
}