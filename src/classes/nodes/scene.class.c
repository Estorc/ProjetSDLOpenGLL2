/**
 * @file
 * @brief This file contains the implementation of the scene class.
 * 
 * This file provides the functionality for managing and rendering scenes
 * in the application, including scene initialization, updating, and cleanup.
 * 
 * @author Loup Picault
 * @date October 27, 2023
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/framebuffer.h"

class Scene : public Node {
    __containerType__ Node *
    public:

    void constructor() {
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void load(FILE *file, Camera **c, Script *scripts) {
        this->type = __type__;
        this::constructor();
        char path[256];
        if (file) {
            fscanf(file,"(%s)\n", 
                path);
        }
        this->children = realloc(this->children, sizeof(Node *));
        this::add_child(load_scene(path, c, scripts));
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }


    void render(mat4 *modelMatrix, Shader activeShader) {
        int modelLoc = glGetUniformLocation(activeShader, "model");
        Mesh *mesh = (Mesh *)this->object;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *) modelMatrix);

        glBindVertexArray(mesh->VAO);

        glDrawArrays(GL_TRIANGLES, 0, mesh->length);
        glBindVertexArray(0);
    }

    
}
