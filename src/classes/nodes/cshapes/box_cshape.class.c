#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class BoxCShape extends CShape {
    __containerType__ Node *

    void constructor(struct BoxCollisionShape *boxCollisionShape) {
        this->object = boxCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void get_priority(int *priority) {
        *priority = 0;
    }

    void load() {
        BoxCollisionShape *boxCollisionShape;
        boxCollisionShape = malloc(sizeof(BoxCollisionShape));
        POINTER_CHECK(boxCollisionShape);
        METHOD_TYPE(this, __type__, constructor, boxCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }








    static VBO _vbo = 0;
    static VAO _vao = 0;
    static Shader collisionShader = 0;
    void get_vao(VAO *vao) {
        if (!_vao) {
            collisionShader = create_shader("shaders/collision_debug.vs", "shaders/collision_debug.fs");
            float boxVertices[] = {
                // Positions          // Normals           // TexCoords  // Tangents          // Bitangents
                // Front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,   0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,   0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,   0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                // Back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,   1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,   0.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,   0.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,   1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,   0.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                // Left face
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,   1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,   0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,   0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,   1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,   0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,   1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                // Right face
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,   1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,   1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                // Top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,   1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                // Bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,   1.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,   0.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,   1.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,   0.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,   1.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            };


            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &_vbo);
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), &boxVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
        }
        *vao = _vao;
    }





    void render(mat4 *modelMatrix) {
        VAO vao;
        METHOD(this, get_vao, &vao);
        use_shader(collisionShader);

        int modelLoc = glGetUniformLocation(collisionShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        
        glBindVertexArray(vao);
        glDepthFunc(GL_ALWAYS);     // Set depth function to always pass
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);       // Restore default depth function
        glBindVertexArray(0);
    }

    
}
