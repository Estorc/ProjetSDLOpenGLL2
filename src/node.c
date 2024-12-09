#include "types.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "render/lighting.h"
#include "classes/classes.h"
#include <stdio.h>



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

void render_node(Node *node, mat4 modelMatrix) {
    
    Shader shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, (int*) &shader);
    set_shader_int(shader, "diffuseMapActive", 0);
    set_shader_int(shader, "normalMapActive", 0);
    set_shader_int(shader, "parallaxMapActive", 0);
    
    glm_translate(modelMatrix, (vec3){node->pos[0], node->pos[1], node->pos[2]});
    glm_rotate(modelMatrix, to_radians(node->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(modelMatrix, to_radians(node->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(modelMatrix, to_radians(node->rot[2]), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(modelMatrix, (vec3){node->scale[0], node->scale[1], node->scale[2]});
    
    switch (node->type) {
        #ifdef DEBUG
        case CLASS_TYPE_POINTLIGHT:         render_point_light(node, modelMatrix);                      break;
        case CLASS_TYPE_DIRECTIONALLIGHT:   render_directional_light(node, modelMatrix);                break;
        case CLASS_TYPE_SPOTLIGHT:          render_directional_light(node, modelMatrix);                break;
        #endif
        default:
            METHOD(node, render, modelMatrix)
            break;
    }
}

#ifdef DEBUG

VBO lightBillboardVBO = 0;
VAO lightBillboardVAO = 0;
Shader billboardShader;
TextureMap lightPointTexture;
TextureMap directionalLightTexture;

const float lightBillboardQuadVertices[] = {
    // positions        // texture Coords
    1.0f,  -1.0f,  0.0f, 1.0f,
    1.0f, 1.0f,  0.0f, 0.0f,
    -1.0f, 1.0f,  1.0f, 0.0f,

    1.0f,  -1.0f,  0.0f, 1.0f,
    -1.0f, 1.0f,  1.0f, 0.0f,
    -1.0f,  -1.0f,  1.0f, 1.0f
};
// setup plane VAO
void init_light_billboard_vao() {
    if (!lightBillboardVAO) {
        lightPointTexture = load_texture_from_path("assets/textures/editor/light_bulb.png", GL_SRGB_ALPHA);
        directionalLightTexture = load_texture_from_path("assets/textures/editor/sun.png", GL_SRGB_ALPHA);
        billboardShader = create_shader("shaders/billboard.vs", "shaders/billboard.fs");
        glGenVertexArrays(1, &lightBillboardVAO);
        glGenBuffers(1, &lightBillboardVBO);
        glBindVertexArray(lightBillboardVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lightBillboardVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lightBillboardQuadVertices), &lightBillboardQuadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }
}

void render_point_light(Node *node, mat4 modelMatrix) {
    init_light_billboard_vao();
    use_shader(billboardShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lightPointTexture);
    
    int modelLoc = glGetUniformLocation(billboardShader, "model");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(lightBillboardVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void render_directional_light(Node *node, mat4 modelMatrix) {
    init_light_billboard_vao();
    use_shader(billboardShader);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, directionalLightTexture);

    int modelLoc = glGetUniformLocation(billboardShader, "model");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(lightBillboardVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
#endif



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
    METHOD(node, free);
}



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
    printf(" - [%s] (%s) %c%c%c%c%c %d children:\n", 
    
            classManager.class_names[node->type],
            "NodeName",

            (!level) ? 'r' : '_', 
            (node->flags & NODE_ACTIVE) ? 'a' : '_', 
            (node->flags & NODE_VISIBLE) ? 'v' : '_', 
            (node->flags & NODE_SCRIPT) ? 's' : '_', 
            (node->shader) ? 'S' : '_', 
            
            node->length);
    for (int i = 0; i < node->length; i++) {
        print_node(node->children[i], level+1);
    }
    #endif
}