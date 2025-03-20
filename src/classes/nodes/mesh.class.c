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

/**
 * @ingroup Classes Classes
 * @{
 */

class Mesh : public Node {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the mesh class.
     *
     * This function initializes a new instance of the mesh class.
     */
    void constructor() {
        this->type = __type__;

        Mesh *mesh;
        mesh = malloc(sizeof(Mesh));
        POINTER_CHECK(mesh);
        create_screen_plane(mesh);

        this->object = mesh;
        SUPER(initialize_node);
    }

    /**
     * @brief Loads the mesh data.
     *
     * This function is responsible for loading the mesh data from a specified source.
     * It initializes the necessary resources and prepares the mesh for rendering.
     *
     * @note Ensure that the source of the mesh data is correctly specified before calling this function.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Saves the current state of the mesh to a file.
     *
     * This function writes the current state of the mesh to the specified file.
     *
     * @param file A pointer to the FILE object where the mesh state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    /**
     * @brief Renders the mesh using the provided model matrix and active shader.
     *
     * This function takes a model matrix and an active shader as parameters and
     * renders the mesh accordingly. The model matrix is used to transform the
     * mesh in the 3D space, and the active shader is used to apply shading effects
     * during the rendering process.
     *
     * @param modelMatrix Mat4 structure representing the model matrix.
     * @param activeShader The shader program to be used for rendering.
     */
    void render(mat4 modelMatrix, Shader activeShader) {
        int modelLoc = glGetUniformLocation(activeShader, "model");
        Mesh *mesh = (Mesh *)this->object;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *) modelMatrix);

        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh->length);
        glBindVertexArray(0);
    }

    
}

