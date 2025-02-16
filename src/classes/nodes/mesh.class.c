/**
 * @file
 * @brief This file defines the implementation of the Mesh class.
 *
 * This file contains the implementation of the Mesh class, which is used to
 * represent and manipulate 3D mesh objects in the application. The Mesh class
 * provides functionalities for loading, rendering, and managing 3D mesh data.
 *
 * @author Loup Picault
 * @date 2023-10-20
 */

#include "raptiquax.h"
#include "classes/classes.h"
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

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *) modelMatrix);

        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh->length);
        glBindVertexArray(0);
    }

    
}
