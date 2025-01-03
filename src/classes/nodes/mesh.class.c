#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Mesh extends Node {
    __containerType__ Node *

    void constructor(struct Mesh *mesh) {
        this->object = mesh;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load() {
        Mesh *mesh;
        mesh = malloc(sizeof(Mesh));
        POINTER_CHECK(mesh);
        create_screen_plane(mesh);
        METHOD_TYPE(this, __type__, constructor, mesh);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }


    void render(mat4 *modelMatrix, Shader activeShader) {
        int modelLoc = glGetUniformLocation(activeShader, "model");
        Mesh *mesh = (Mesh *)this->object;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh->length);
        glBindVertexArray(0);
    }

    
}
