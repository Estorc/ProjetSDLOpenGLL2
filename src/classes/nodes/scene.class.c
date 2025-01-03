#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/framebuffer.h"

class Scene extends Node {
    __containerType__ Node *

    void constructor() {
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file, Camera **c, Script *scripts) {
        METHOD_TYPE(this, __type__, constructor);
        char path[256];
        if (file) {
            fscanf(file,"(%s)\n", 
                path);
        }
        this->children = realloc(this->children, sizeof(Node *));
        add_child(this, load_scene(path, c, scripts));
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
