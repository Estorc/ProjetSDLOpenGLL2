#include "types.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "utils/node_tools.h"
#include <stdio.h>

/**
 * Create a node with the desired type
 *
 * @param node {Node*} - The node pointer.
 * @param type {u8} - The node type code.
 * @param data {void*} - The raw data of the node.
 * @returns {Node*} The node pointer.
 */

Node * create_node(Node *node, u8 type, void *data) {
    switch (type) {
        case NODE_MODEL:                    return create_model_node(node, (Model *) data);
        case NODE_TEXTURED_MESH:            return create_textured_mesh_node(node, (TexturedMesh *) data);
        case NODE_SKYBOX:                   return create_skybox_node(node, (TexturedMesh *) data);
        case NODE_MESH:                     return create_mesh_node(node, (Mesh *) data);
        case NODE_FRAMEBUFFER:              return create_framebuffer_node(node, (FrameBuffer *) data);
        case NODE_TEXTURE:                  return create_texture_node(node, (TextureMap *) data);
        case NODE_VIEWPORT:                 return create_viewport_node(node, (Viewport *) data);
        case NODE_FILTER:                   return create_filter_node(node, (Filter *) data);
        case NODE_STATIC_BODY:              return create_static_body_node(node, (StaticBody *) data);
        case NODE_RIGID_BODY:               return create_rigid_body_node(node, (RigidBody *) data);
        case NODE_KINEMATIC_BODY:           return create_kinematic_body_node(node, (KinematicBody *) data);
        case NODE_BOX_CSHAPE:      return create_box_collision_shape_node(node, (BoxCollisionShape *) data);
        case NODE_SPHERE_CSHAPE:   return create_sphere_collision_shape_node(node, (SphereCollisionShape *) data);
        case NODE_PLANE_CSHAPE:    return create_plane_collision_shape_node(node, (PlaneCollisionShape *) data);
        case NODE_CAMERA:                   return create_camera_node(node, (Camera *) data);
    }
    return NULL;
}



/**
 * Initializes a Node structure by setting default values for its properties.
 *
 * @param node {Node*} A pointer to the Node structure to be initialized.
 * @return {Node*} The pointer to the initialized Node structure.
 *
 * This function sets up a Node object with default values, including resetting 
 * its transformation properties (position, rotation, scale) and setting default 
 * flags and child relationships. This is typically used when creating or resetting 
 * a node in a scene graph or object hierarchy.
 *
 * Example Usage:
 * Node myNode;
 * initialize_node(&myNode);
 *
 * Return:
 * Returns the pointer to the Node structure passed to the function, fully initialized 
 * and ready for use.
 */

Node * initialize_node(Node *node) {
    node->flags = NODE_DEFAULT_FLAGS;
    node->length = 0;
    node->children = NULL;
    node->script = NULL;
    node->params = NULL;
    node->params_count = 0;
    Vec3fZero(node->pos);
    Vec3fZero(node->rot);
    Vec3fOne(node->scale);
    return node;
}

/**
 * Initializes a Node structure for a camera object.
 *
 * @param node {Node*} - The node pointer.
 * @param camera {Camera*} - The camera object.
 * @returns {Node*} The node pointer.
 */

Node * create_camera_node(Node *node, Camera *camera) {
    node->object = camera;
    node->type = NODE_CAMERA;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a static body object.
 *
 * @param node {Node*} - The node pointer.
 * @param staticBody {StaticBody*} - The static body object.
 * @returns {Node*} The node pointer.
 */

Node * create_static_body_node(Node *node, StaticBody *staticBody) {
    node->object = staticBody;
    node->type = NODE_STATIC_BODY;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a rigid body object.
 *
 * @param node {Node*} - The node pointer.
 * @param rigidBody {RigidBody*} - The rigid body object.
 * @returns {Node*} The node pointer.
 */

Node * create_rigid_body_node(Node *node, RigidBody *rigidBody) {
    node->object = rigidBody;
    node->type = NODE_RIGID_BODY;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a kinematic body object.
 *
 * @param node {Node*} - The node pointer.
 * @param kinematicBody {KinematicBody*} - The kinematic body object.
 * @returns {Node*} The node pointer.
 */

Node * create_kinematic_body_node(Node *node, KinematicBody *kinematicBody) {
    node->object = kinematicBody;
    node->type = NODE_KINEMATIC_BODY;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a plane collision shape object.
 *
 * @param node {Node*} - The node pointer.
 * @param planeCollisionShape {PlaneCollisionShape*} - The plane collision shape object.
 * @returns {Node*} The node pointer.
 */

Node * create_plane_collision_shape_node(Node *node, PlaneCollisionShape *planeCollisionShape) {
    node->object = planeCollisionShape;
    node->type = NODE_PLANE_CSHAPE;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a box collision shape object.
 *
 * @param node {Node*} - The node pointer.
 * @param boxCollisionShape {BoxCollisionShape*} - The box collision shape object.
 * @returns {Node*} The node pointer.
 */

Node * create_box_collision_shape_node(Node *node, BoxCollisionShape *boxCollisionShape) {
    node->object = boxCollisionShape;
    node->type = NODE_BOX_CSHAPE;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a sphere collision shape object.
 *
 * @param node {Node*} - The node pointer.
 * @param sphereCollisionShape {SphereCollisionShape*} - The sphere collision shape object.
 * @returns {Node*} The node pointer.
 */

Node * create_sphere_collision_shape_node(Node *node, SphereCollisionShape *sphereCollisionShape) {
    node->object = sphereCollisionShape;
    node->type = NODE_SPHERE_CSHAPE;
    return initialize_node(node);
}

/**
 * Initializes an empty Node structure.
 *
 * @param node {Node*} - The node pointer.
 * @returns {Node*} The node pointer.
 */

Node * create_empty_node(Node *node) {
    node->object = NULL;
    node->type = NODE_EMPTY;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a model object.
 *
 * @param node {Node*} - The node pointer.
 * @param model {Model*} - The model object.
 * @returns {Node*} The node pointer.
 */

Node * create_model_node(Node *node, Model *model) {
    node->object = model;
    node->type = NODE_MODEL;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a textured mesh object.
 *
 * @param node {Node*} - The node pointer.
 * @param texturedMesh {TexturedMesh*} - The textured mesh object.
 * @returns {Node*} The node pointer.
 */

Node * create_textured_mesh_node(Node *node, TexturedMesh *texturedMesh) {
    node->object = texturedMesh;
    node->type = NODE_TEXTURED_MESH;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a skybox object.
 *
 * @param node {Node*} - The node pointer.
 * @param texturedMesh {TexturedMesh*} - The skybox's textured mesh object.
 * @returns {Node*} The node pointer.
 */

Node * create_skybox_node(Node *node, TexturedMesh *texturedMesh) {
    node->object = texturedMesh;
    node->type = NODE_SKYBOX;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a mesh object.
 *
 * @param node {Node*} - The node pointer.
 * @param mesh {Mesh*} - The mesh object.
 * @returns {Node*} The node pointer.
 */

Node * create_mesh_node(Node *node, Mesh *mesh) {
    node->object = mesh;
    node->type = NODE_MESH;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a framebuffer object.
 *
 * @param node {Node*} - The node pointer.
 * @param frameBuffer {FrameBuffer*} - The framebuffer object.
 * @returns {Node*} The node pointer.
 */

Node * create_framebuffer_node(Node *node, FrameBuffer *frameBuffer) {
    node->object = frameBuffer;
    node->type = NODE_FRAMEBUFFER;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a texture object.
 *
 * @param node {Node*} - The node pointer.
 * @param texture {TextureMap*} - The texture object.
 * @returns {Node*} The node pointer.
 */

Node * create_texture_node(Node *node, TextureMap *texture) {
    node->object = texture;
    node->type = NODE_TEXTURE;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a viewport object.
 *
 * @param node {Node*} - The node pointer.
 * @param viewport {Viewport*} - The viewport object.
 * @returns {Node*} The node pointer.
 */

Node * create_viewport_node(Node *node, Viewport *viewport) {
    node->object = viewport;
    node->type = NODE_VIEWPORT;
    return initialize_node(node);
}

/**
 * Initializes a Node structure for a filter object.
 *
 * @param node {Node*} - The node pointer.
 * @param filter {Filter*} - The filter object.
 * @returns {Node*} The node pointer.
 */

Node * create_filter_node(Node *node, Filter *filter) {
    node->object = filter;
    node->type = NODE_FILTER;
    return initialize_node(node);
}







int index_of_child(Node *node, Node *child) {
    for (int i = 0; i < node->length; i++) {
        if (child == node->children[i]) return i;
    }
    return -1;
}

/**
 * Add a child to a node
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The child node.
 */

void add_child(Node *node, Node *child) {
    
    node->children[node->length++] = child;
    child->parent = node;
    
}

/**
 * Add a child to a node and realloc the parent
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The child node.
 */

void add_child_and_realloc(Node *node, Node *child) {
    node->children = realloc(node->children, sizeof(Node *) * (node->length+1));
    POINTER_CHECK(node->children);
    node->children[node->length++] = child;
    child->parent = node;
}

/**
 * Remove a child from the parent
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The child node.
 */

void remove_child(Node *node, Node *child) {
    for (Node **rightCursor, **leftCursor = rightCursor = node->children; rightCursor < node->children + node->length; leftCursor++, rightCursor++) {
        if (child == *rightCursor) rightCursor++;
        if (leftCursor != rightCursor && rightCursor < node->children + node->length) *leftCursor = *rightCursor;
    }
}

/**
 * Remove a child from the parent and realloc the parent
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The children node.
 */

void remove_child_and_realloc(Node *node, Node *child) {
    for (Node **rightCursor, **leftCursor = rightCursor = node->children; rightCursor < node->children + node->length; leftCursor++, rightCursor++) {
        if (child == *rightCursor) rightCursor++;
        if (leftCursor != rightCursor && rightCursor < node->children + node->length) *leftCursor = *rightCursor;
    }
    node->children = realloc(node->children, sizeof(Node *) * (--node->length));
    //POINTER_CHECK(node->children);
}

/**
 * Remove a child from the parent and free the child
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The children node.
 */

void remove_child_and_free(Node *node, Node *child) {
    for (Node **rightCursor, **leftCursor = rightCursor = node->children; rightCursor < node->children + node->length; leftCursor++, rightCursor++) {
        if (child == *rightCursor) free_node(*rightCursor), rightCursor++;
        if (leftCursor != rightCursor && rightCursor < node->children + node->length) *leftCursor = *rightCursor;
    }
}

/**
 * Remove a child from the parent, realloc the parent and free the child
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The children node.
 */

void remove_child_and_free_and_realloc(Node *node, Node *child) {
    for (Node **rightCursor, **leftCursor = rightCursor = node->children; rightCursor < node->children + node->length; leftCursor++, rightCursor++) {
        if (child == *rightCursor) {
            free_node(*rightCursor);
            rightCursor++;
        }
        if (leftCursor != rightCursor && rightCursor < node->children + node->length) *leftCursor = *rightCursor;
    }
    node->children = realloc(node->children, sizeof(Node *) * (--node->length));
    //POINTER_CHECK(node->children);
}




/**
 * Renders a 3D node with applied transformations.
 * 
 * @param node {Node*} - Pointer to the Node structure containing object data.
 * @param shaders {Shader*} - Array of shaders for rendering different node types.
 * @param modelMatrix {mat4} - Transformation matrix to be applied to the node.
 * 
 * The function applies translation, rotation, and scaling to the modelMatrix
 * based on the node's properties, disables normal and displacement maps in
 * the current shader, and then renders the node using the appropriate function
 * based on its type (e.g., model, textured mesh, skybox).
 */

void render_node(Node *node, Shader *shaders, mat4 modelMatrix) {
    
    Shader shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, (int*) &shader);
    set_shader_int(shader, "normalMapActive", 0);
    set_shader_int(shader, "displacementMapActive", 0);
    
    glm_translate(modelMatrix, (vec3){node->pos[0], node->pos[1], node->pos[2]});
    glm_rotate(modelMatrix, to_radians(node->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(modelMatrix, to_radians(node->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(modelMatrix, to_radians(node->rot[2]), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(modelMatrix, (vec3){node->scale[0], node->scale[1], node->scale[2]});
    
    switch (node->type) {
        case NODE_MODEL:            render_model(node, modelMatrix);                            break;
        case NODE_TEXTURED_MESH:    render_textured_mesh(node, modelMatrix);                    break;
        case NODE_MESH:             render_mesh(node, modelMatrix);                             break;
        case NODE_SKYBOX:           render_skybox(node, shaders[SHADER_SKYBOX], modelMatrix);   break;
    }
}

/**
 * Renders a 3D model represented by a Node structure.
 * 
 * @param node {Node*} - Pointer to the Node structure containing the model data.
 * @param modelMatrix {mat4} - Transformation matrix to be applied to the model during rendering.
 * 
 * This function retrieves the currently active shader program and sends the model's transformation 
 * matrix to the shader. It then iterates through each object in the model and its associated materials. 
 * For each material, the function sets the appropriate shader uniforms for ambient, specular, diffuse colors, 
 * and shininess. It also binds the relevant textures (diffuse, normal, displacement) if available and 
 * renders the object using `glDrawArrays`. The function ensures that textures and shaders are correctly 
 * configured before rendering each part of the model.
 */

void render_model(Node *node, mat4 modelMatrix) {
    Shader shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, (int*) &shader);
    
    int modelLoc = glGetUniformLocation(shader, "model");
    Model *model = (Model *) node->object;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
    if (model) for (int j = 0; j < model->length; j++) {
        u32 objectPosition = 0;
        glBindVertexArray(model->objects[j].VAO);
        for (int k = 0; k < model->objects[j].materialsCount; k++) {
            glActiveTexture(GL_TEXTURE0);

            glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, &model->objects[j].materials[k]->ambientCol);
            glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, &model->objects[j].materials[k]->specularCol);
            glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, &model->objects[j].materials[k]->diffuseCol);
            glUniform1fv(glGetUniformLocation(shader, "material.shininess"), 1, &model->objects[j].materials[k]->specularExp);
            if (model->objects[j].materials[k]->diffuseTextureMap) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->diffuseTextureMap);
            }
            if (model->objects[j].materials[k]->normalTextureMap) {
                set_shader_int(shader, "normalMapActive", 1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->normalTextureMap);
            }
            if (model->objects[j].materials[k]->displacementTextureMap) {
                set_shader_int(shader, "displacementMapActive", 1);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->displacementTextureMap);
            }

            
            glDrawArrays(GL_TRIANGLES, objectPosition, model->objects[j].materialsLength[k] * 3);
            
            objectPosition += model->objects[j].materialsLength[k] * 3;
            
        }
    }
    glBindVertexArray(0);
    
}

/**
 * Renders a textured mesh represented by a Node structure.
 * 
 * @param node {Node*} - Pointer to the Node structure containing the textured mesh data.
 * @param modelMatrix {mat4} - Transformation matrix to be applied to the textured mesh during rendering.
 * 
 * This function retrieves the currently active shader program and sets the model's transformation 
 * matrix in the shader. It then activates the texture unit, binds the texture associated with the 
 * textured mesh, and binds the Vertex Array Object (VAO) of the mesh. The function finally renders 
 * the mesh using `glDrawArrays`, and unbinds the VAO after rendering.
 */

void render_textured_mesh(Node *node, mat4 modelMatrix) {
    Shader shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, (int*) &shader);

    int modelLoc = glGetUniformLocation(shader, "model");
    TexturedMesh *texturedMesh = (TexturedMesh *)node->object;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
    // render Cube
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturedMesh->texture);
    glBindVertexArray(texturedMesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, texturedMesh->length);
    glBindVertexArray(0);
}

/**
 * Renders a skybox represented by a Node structure.
 * 
 * @param node {Node*} - Pointer to the Node structure containing the skybox's textured mesh data.
 * @param shader {Shader} - Shader program to be used for rendering the skybox.
 * @param modelMatrix {mat4} - Transformation matrix to be applied to the skybox during rendering.
 * 
 * This function modifies the depth function to ensure the skybox is rendered correctly at the farthest depth 
 * and uses the provided shader for rendering. It sets the model's transformation matrix in the shader, 
 * activates the texture unit, binds the skybox's texture and VAO, and renders the skybox using `glDrawArrays`. 
 * After rendering, the function resets the depth function to its default state.
 */

void render_skybox(Node *node, Shader shader, mat4 modelMatrix) {
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    use_shader(shader);

    int modelLoc = glGetUniformLocation(shader, "model");
    TexturedMesh *texturedMesh = (TexturedMesh *)node->object;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
    // render Cube
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texturedMesh->texture);
    set_shader_int(shader, "skybox", 0);
    glBindVertexArray(texturedMesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, texturedMesh->length);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 * Renders a basic mesh represented by a Node structure.
 * 
 * @param node {Node*} - Pointer to the Node structure containing the mesh data.
 * @param modelMatrix {mat4} - Transformation matrix to be applied to the mesh during rendering.
 * 
 * This function retrieves the currently active shader program and sets the model's transformation 
 * matrix in the shader. It then binds the Vertex Array Object (VAO) of the mesh and renders it 
 * using `glDrawArrays`. After rendering, the VAO is unbound.
 */


void render_mesh(Node *node, mat4 modelMatrix) {
    Shader shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, (int*) &shader);
    
    int modelLoc = glGetUniformLocation(shader, "model");
    Mesh *mesh = (Mesh *)node->object;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(mesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh->length);
    glBindVertexArray(0);
}

/**
 * Frees the memory associated with a Node structure and its children.
 * 
 * @param node {Node*} - Pointer to the Node structure to be freed.
 * 
 * This function recursively frees all child nodes of the given node. It handles different 
 * node types, freeing associated resources specific to each type (e.g., models, viewports, 
 * rigid bodies). It also frees the object's memory, the children array, and finally the 
 * node itself. This ensures proper memory management and prevents memory leaks.
 */

void free_node(Node *node) {
    for (int i = 0; i < node->length; i++) {
        free_node(node->children[i]);
    }

    switch (node->type) {
        case NODE_MODEL:
            break;
        case NODE_VIEWPORT:
            Viewport *viewport = (Viewport *) node->object;
            if (viewport->scene)
                free_node(viewport->scene);
            free(node->object);
            break;
        case NODE_RIGID_BODY:
        case NODE_STATIC_BODY:
            {
                u8 *length;
                Node ***shapes;
                GET_FROM_BODY_NODE(node, length, length);
                GET_FROM_BODY_NODE(node, collisionsShapes, shapes);
                for (int j = 0; j < *length; j++) {
                    free((*shapes)[j]);
                }
                free(*shapes);
            }
            free(node->object);
            break;
        default:
            free(node->object);
            break;
    }

    free(node->params);
    free(node->children);
    free(node);
}


#ifdef DEBUG
const char NodeTypeNames[][100] = {
    NODE_NAMES
};
#endif



/**
 * Prints the details of a Node structure and its children to the console.
 * 
 * @param node {Node*} - Pointer to the Node structure to be printed.
 * @param level {int} - Current depth level in the node hierarchy for indentation.
 * 
 * This function is used for debugging purposes. It prints the node's address, type, 
 * active and visible flags, and the number of children. The output is indented according 
 * to the node's level in the hierarchy to visually represent the tree structure of the nodes. 
 * The function recursively prints all child nodes, providing a full view of the node tree.
 */

void print_node(Node *node, int level) {
    #ifdef DEBUG
    for (int i = 0; i < level; i++) printf("\t");
    printf(" - %c%c%c%c %p [%s] %d children:\n", 
    
            (!level) ? 'r' : '_', 
            (node->flags & NODE_ACTIVE) ? 'a' : '_', 
            (node->flags & NODE_VISIBLE) ? 'v' : '_', 
            (node->flags & NODE_SCRIPT) ? 's' : '_', 
            
            node, NodeTypeNames[node->type], node->length);
    for (int i = 0; i < node->length; i++) {
        print_node(node->children[i], level+1);
    }
    #endif
}