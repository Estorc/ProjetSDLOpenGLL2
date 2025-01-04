#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/lighting.h"
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
        if (!vao) METHOD(this, init_vao);
        use_shader(billboardShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lightPointTexture);
        
        int modelLoc = glGetUniformLocation(billboardShader, "model");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
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
}