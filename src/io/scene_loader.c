#include <stdlib.h>
#include <stdio.h>
#include "stringio.h"
#include "../types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>
#include "../math/math_util.h"
#include "model.h"
#include "../render/framebuffer.h"
#include "../render/lighting.h"
#include "../physics/bodies.h"
#include "../storage/node.h"
#include "../render/render.h"
#include "../window.h"
#include "input.h"
#include "../render/camera.h"
#include "shader.h"
#include "../render/depth_map.h"
#include "node_loader.h"
#include "../buffer.h"
#include "../classes/classes.h"


/**
 * Loads a node from a file and initializes its properties based on the data found in the file.
 * 
 * @param file {FILE*} Pointer to the file from which the node will be loaded.
 * @param c {Camera**} Pointer to a Camera pointer that will be set if a camera node is loaded.
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

Node *load_node(FILE *file, Camera **c, Script scripts[SCRIPTS_COUNT], Node *editor) {
    
    char symbol[100];
    Node *node;
    
    node = malloc(sizeof(Node));
    POINTER_CHECK(node);
    
    fscanf(file,"%100[a-z,A-Z]", symbol);

    malloc_node(node, find_string_index(symbol, (const char **) classManager.class_names, CLASS_TYPE_COUNT), file, c, scripts, editor);
    

    #ifdef DEBUG
    //print_node(node,0);
    #endif
    char paramSymbol;
    do {
        
        paramSymbol = getc(file);
        
        switch (paramSymbol) {
            case ':': ;
                
                int children_count = 0;
                fscanf(file,"%d\n", &children_count);
                
                node->children = realloc(node->children, sizeof(Node *) * children_count);
                POINTER_CHECK(node->children);
                
                for (int i = 0; i < children_count; i++)
                    add_child(node, load_node(file, c, scripts, editor));
                break;
            case '[': ;
                char transformSymbol;
                do {
                    
                    transformSymbol = getc(file);
                    
                    float x,y,z;
                    u8 value;
                    switch (transformSymbol) {
                        case 's': ;
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->scale[0] = x;
                            node->scale[1] = y;
                            node->scale[2] = z;
                            break;
                        case 'r': ;
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->rot[0] = x;
                            node->rot[1] = y;
                            node->rot[2] = z;
                            break;
                        case 'm': ;
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->pos[0] = x;
                            node->pos[1] = y;
                            node->pos[2] = z;
                            break;
                        case 'v': ;
                            fscanf(file, "%hhd", &value);
                            if (value)
                                node->flags |= NODE_VISIBLE;
                            else
                                node->flags &= ~NODE_VISIBLE;
                            break;
                        case 'a': ;
                            fscanf(file, "%hhd", &value);
                            if (value)
                                node->flags |= NODE_ACTIVE;
                            else
                                node->flags &= ~NODE_ACTIVE;
                            break;
                    }
                } while (transformSymbol != ']');
                break;
            case '{': ;
                char scriptname[100];
                fscanf(file, "%100[^}]}", scriptname);
                for (int i = 0; i < SCRIPTS_COUNT; i++) {
                    if (!strcmp(scripts[i].name, scriptname)) {
                        node->flags |= NODE_SCRIPT;
                        node->script = scripts[i].script;
                        break;
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

Node *load_scene(char *path, Camera **c, Script *scripts) {
    buffers.collisionBuffer.length = 0;
    buffers.lightingBuffer.length = 0;
    buffers.collisionBuffer.index = 0;
    buffers.lightingBuffer.index = 0;
    FILE * file = fopen(path, "r");

    if (!file) return NULL;

    Node *root;
    root = load_node(file, c, scripts, 0);
    if (!root) {
        fclose(file);
        return NULL;
    }
    root->parent = NULL;
    print_node(root, 0);

    buffers.collisionBuffer.collisionsShapes = realloc(buffers.collisionBuffer.collisionsShapes, sizeof(Node *) * buffers.collisionBuffer.length);
    // Check if the memory allocation was successful

    buffers.lightingBuffer.lightings = realloc(buffers.lightingBuffer.lightings, sizeof(Node *) * buffers.lightingBuffer.length);
    // Check if the memory allocation was successful

    fclose(file);

    return root;

}