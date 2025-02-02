#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Mesh : public Node {
    __containerType__ Node *
    public:

    void constructor(struct Mesh *mesh) {
        this->object = mesh;
        this->type = __type__;
        SUPER(initialize_node);
    }


    void load() {
        Mesh *mesh;
        mesh = malloc(sizeof(Mesh));
        POINTER_CHECK(mesh);
        create_screen_plane(mesh);
        this->type = __type__;
        this::constructor(mesh);
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
