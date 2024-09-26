#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>
#include "../math/math_util.h"
#include "model.h"
#include "../render/framebuffer.h"
#include "../physics/bodies.h"
#include "../node.h"
#include "../render/render.h"
#include "../window.h"
#include "input.h"
#include "../render/camera.h"
#include "shader.h"
#include "../render/depth_map.h"
#include "node_loader.h"


/**
 * Loads a node from a file and initializes its properties based on the data found in the file.
 * 
 * @param file {FILE*} Pointer to the file from which the node will be loaded.
 * @param c {Camera**} Pointer to a Camera pointer that will be set if a camera node is loaded.
 * @param collisionBuffer {CollisionBuffer*} Pointer to a CollisionBuffer structure for managing collision shapes.
 * 
 * @return {Node*} Returns a pointer to the newly created Node. The node's properties and type 
 *                 are determined by the data read from the file.
 * 
 * This function reads from a file to construct a scene graph node. It handles various types of nodes,
 * including cameras, collision shapes, rigid bodies, static bodies, meshes, models, textured meshes,
 * skyboxes, viewports, and framebuffers. The node's scale, rotation, position, and visibility flags
 * are also configured based on parameters specified in the file. If a camera node is created, it updates
 * the provided Camera pointer. The function also manages the allocation and initialization of related
 * structures and checks for memory allocation failures.
 */

Node *load_node(FILE *file, Camera **c, CollisionBuffer *collisionBuffer, Script scripts[SCRIPTS_COUNT], Node *editor) {
    
    char symbol[10];
    Node *node;
    
    node = malloc(sizeof(Node));
    POINTER_CHECK(node);
    
    fscanf(file,"%10[a-z]", symbol);
    if (!strcmp(symbol, "empty")) {
        malloc_node(node, NODE_EMPTY, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "camera")) {
        malloc_node(node, NODE_CAMERA, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "cbox")) {
        malloc_node(node, NODE_BOX_CSHAPE, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "cplane")) {
        malloc_node(node, NODE_PLANE_CSHAPE, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "csphere")) {
        malloc_node(node, NODE_SPHERE_CSHAPE, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "kbody")) {
        malloc_node(node, NODE_KINEMATIC_BODY, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "rbody")) {
        malloc_node(node, NODE_RIGID_BODY, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "sbody")) {
        malloc_node(node, NODE_STATIC_BODY, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "sp")) {
        malloc_node(node, NODE_MESH, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "m")) {
        malloc_node(node, NODE_MESH, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "mdl")) {
        malloc_node(node, NODE_MODEL, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "tp")) {
        malloc_node(node, NODE_TEXTURED_MESH, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "sky")) {
        malloc_node(node, NODE_SKYBOX, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "v")) {
        malloc_node(node, NODE_VIEWPORT, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "msaa")) {
        malloc_node(node, NODE_FRAMEBUFFER, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "plight")) {
        malloc_node(node, NODE_POINT_LIGHT, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "dlight")) {
        malloc_node(node, NODE_DIRECTIONAL_LIGHT, file, c, collisionBuffer, scripts, editor);
    }
    if (!strcmp(symbol, "slight")) {
        malloc_node(node, NODE_SPOT_LIGHT, file, c, collisionBuffer, scripts, editor);
    }
    
    #ifdef DEBUG
    //print_node(node,0);
    #endif
    char paramSymbol;
    do {
        
        paramSymbol = getc(file);
        
        switch (paramSymbol) {
            case ':':
                
                int children_count = 0;
                fscanf(file,"%d\n", &children_count);
                
                node->children = realloc(node->children, sizeof(Node *) * children_count);
                POINTER_CHECK(node->children);
                
                for (int i = 0; i < children_count; i++)
                    add_child(node, load_node(file, c, collisionBuffer, scripts, editor));
                break;
            case '[':
                char transformSymbol;
                do {
                    
                    transformSymbol = getc(file);
                    
                    float x,y,z;
                    u8 value;
                    switch (transformSymbol) {
                        case 's':
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->scale[0] = x;
                            node->scale[1] = y;
                            node->scale[2] = z;
                            break;
                        case 'r':
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->rot[0] = x;
                            node->rot[1] = y;
                            node->rot[2] = z;
                            break;
                        case 'm':
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->pos[0] = x;
                            node->pos[1] = y;
                            node->pos[2] = z;
                            break;
                        case 'v':
                            fscanf(file, "%hhd", &value);
                            if (value)
                                node->flags |= NODE_VISIBLE;
                            else
                                node->flags &= ~NODE_VISIBLE;
                            break;
                        case 'a':
                            fscanf(file, "%hhd", &value);
                            if (value)
                                node->flags |= NODE_ACTIVE;
                            else
                                node->flags &= ~NODE_ACTIVE;
                            break;
                    }
                } while (transformSymbol != ']');
                break;
            case '{':
                char scriptname[100];
                fscanf(file, "%100[^}]}", scriptname);
                for (int i = 0; i < SCRIPTS_COUNT; i++) {
                    if (!strcmp(scripts[i].name, scriptname)) {
                        node->flags |= NODE_SCRIPT;
                        node->script = scripts[i].script;
                    }
                        
                }
                break;
        }
    } while (paramSymbol != '\n' && paramSymbol != ':' && paramSymbol != -1);
    
    return node;
}

/**
 * Loads a scene from a specified file path and initializes a scene graph structure.
 * 
 * @param path {char*} - Path to the scene file to be loaded.
 * @param c {Camera**} - Pointer to a Camera pointer that will be set if a camera node is loaded.
 * @param collisionBuffer {CollisionBuffer*} - Pointer to a CollisionBuffer structure for managing collision shapes.
 * 
 * @return Node* Returns a pointer to the root Node of the loaded scene graph.
 *               If the file cannot be opened, returns NULL.
 * 
 * This function opens a scene file specified by the `path` parameter, reads the contents, 
 * and constructs a scene graph using the `load_node` function. The scene graph's root node
 * is created first, and then it creates a viewport node that holds the reference to the root scene.
 * The function also allocates memory for collision shapes based on the number of shapes encountered 
 * during the loading process. It prints the structure of the loaded nodes for debugging purposes.
 * If any allocation fails during the loading process, appropriate error handling should be considered.
 */

Node *load_scene(char *path, Camera **c, CollisionBuffer *collisionBuffer, Script *scripts) {
    if (collisionBuffer) collisionBuffer->length = 0;
    FILE * file = fopen(path, "r");

    if (!file) return NULL;

    Node *root;
    root = load_node(file, c, collisionBuffer, scripts, 0);
    if (!root) {
        fclose(file);
        return NULL;
    }
    print_node(root, 0);

    Node *node;
    node = load_node(file, c, collisionBuffer, scripts, 0);
    if (!node) {
        free_node(root);
        fclose(file);
        return NULL;
    }

    Viewport *viewport = (Viewport *) node->object;
    viewport->scene = root;
    root->parent = node;
    node->parent = NULL;
    print_node(node, 0);
    if (collisionBuffer) {
        collisionBuffer->collisionsShapes = malloc(sizeof(Node *) * collisionBuffer->length);
        if (!collisionBuffer->collisionsShapes) {
            free_node(root);
            free_node(node);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);

    return node;

}