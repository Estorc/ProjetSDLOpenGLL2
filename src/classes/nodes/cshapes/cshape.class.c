/**
 * @file
 * @brief This file defines the implementation of various CShape classes.
 *
 * This source file contains the implementation of different shapes used in the project.
 * It provides the necessary functions to create, manipulate, and render these shapes
 * using SDL and OpenGL. The shapes include basic geometric forms such as circles,
 * rectangles, and polygons.
 *
 * It is designed to facilitate the creation and management of shapes within the
 * graphical application, ensuring efficient rendering and interaction.
 *
 * @author Loup Picault
 * @date 2023-10-25
 */

#include <raptiquax.h>
#include <classes/classes.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <storage/node.h>

/**
 * @ingroup Classes Classes
 * @{
 */
class CShape : public Node {
    __containerType__ Node *
    public:

    /**
     * @brief Checks if the given shape is a cshape.
     *
     * This function sets the value of the boolean pointer to indicate whether the shape is a cshape.
     *
     * @param cshape A pointer to a boolean variable that will be set to true if the shape is a cshape, false otherwise.
     */
    void is_cshape(bool *cshape) {
        (*cshape) = true;
    }

    /**
     * @brief Retrieves the model associated with the CShape.
     *
     * This function assigns the model associated with the CShape to the provided
     * pointer. The model is returned as a double pointer to allow modification
     * of the original pointer passed by the caller.
     *
     * @param[out] model A double pointer to a Model structure where the model
     *                   associated with the CShape will be stored.
     */
    void get_model(Model **model) {
        *model = NULL;
    }

    /**
     * @brief Shader used for collision detection.
     *
     * @details This static variable holds the shader program used to detect collisions 
     * between shapes. The shader is initialized and used within the CShape class 
     * to perform collision detection operations.
     */
    static Shader collisionShader;

    /**
     * @brief Retrieves the shader information.
     *
     * This function is responsible for obtaining the shader information and 
     * storing it in the provided Shader structure.
     *
     * @param shader A pointer to a Shader structure where the shader information 
     *               will be stored.
     */
    void get_shader(Shader *shader) {
        if (!collisionShader) {
            collisionShader = create_shader("shaders/collision_debug.vs", "shaders/collision_debug.fs");
        }
        *shader = collisionShader;
    }


    /**
     * @brief Prepares the rendering process by setting up the model matrix and activating the shader.
     *
     * This function is responsible for preparing the necessary components for rendering a shape.
     * It takes a model matrix and an active shader as parameters and performs the required setup.
     *
     * @param modelMatrix Model matrix (of type mat4) that will be used for rendering.
     *                    The model matrix contains the transformations (translation, rotation, scaling)
     *                    that will be applied to the shape before rendering.
     * @param activeShader The shader program (of type Shader) that will be used for rendering.
     *                     The shader program contains the vertex and fragment shaders that will be
     *                     applied to the shape during rendering.
     */
    void prepare_render(mat4 modelMatrix, Shader activeShader) {
        glm_mat4_identity(modelMatrix);
        
        glm_translate(modelMatrix, (vec3){this->globalPos[0], this->globalPos[1], this->globalPos[2]});
        glm_rotate(modelMatrix, to_radians(this->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(modelMatrix, to_radians(this->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(modelMatrix, to_radians(this->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});
        glm_scale(modelMatrix, (vec3){this->globalScale[0], this->globalScale[1], this->globalScale[2]});
    }


    /**
     * @brief Renders the shape using the provided model matrix and active shader.
     *
     * This function takes a model matrix and an active shader as parameters and
     * renders the shape accordingly. The model matrix is used to transform the
     * shape in the 3D space, and the active shader is used to apply shading effects
     * during the rendering process.
     *
     * @param modelMatrix Mat4 structure representing the model matrix.
     *                    This matrix is used to transform the shape in the 3D space.
     * @param activeShader An instance of the Shader structure representing the active shader.
     *                     This shader is used to apply shading effects during the rendering process.
     */
    void render(mat4 modelMatrix, Shader activeShader) {
        Model *model;
        this::get_model(&model);
        Shader shader;
        this::get_shader(&shader);
        use_shader(shader);
        if (model) render_model(modelMatrix, shader, model);
        use_shader(activeShader);
    }

}

