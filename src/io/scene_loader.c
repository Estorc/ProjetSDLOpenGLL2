#include <raptiquax.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <render/lighting.h>
#include <physics/bodies.h>
#include <storage/node.h>
#include <render/render.h>
#include <window.h>
#include <io/input.h>
#include <render/camera.h>
#include <io/shader.h>
#include <render/depth_map.h>
#include <io/node_loader.h>
#include <buffer.h>
#include <classes/classes.h>


Node *load_node(FILE *file, Camera **c, Script *scripts, Node *editor) {
    
    char symbol[100];
    Node *node;
    
    node = malloc(sizeof(Node));
    POINTER_CHECK(node);
    
    fscanf(file,"%99[a-z,A-Z]", symbol);

    malloc_node(node, find_string_index(symbol, classManager.class_names, CLASS_TYPE_COUNT), file, c, scripts, editor);
    
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
                    node::add_child(load_node(file, c, scripts, editor));
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
                char scriptname[BEHAVIOR_SCRIPT_COUNT][100];
                Behavior *behavior = malloc(sizeof(Behavior));
                memset(behavior, 0, sizeof(Behavior));
                for (int i = 0; i < BEHAVIOR_SCRIPT_COUNT; i++) {
                    if (i == BEHAVIOR_SCRIPT_COUNT-1) fscanf(file, "%100[^}]}", scriptname[i]);
                    else fscanf(file, "%100[^,],", scriptname[i]);
                    for (int j = 0; j < __scriptIndex__; j++) {
                        if (!strcmp(scripts[j].name, scriptname[i])) {
                            node->flags |= NODE_SCRIPT;
                            (*behavior)[i] = scripts[j].script;
                        }
                    }
                }
                if (node->flags & NODE_SCRIPT) node->behavior = behavior;
                else free(behavior);
                break;
        }
    } while (paramSymbol != '\n' && paramSymbol != ':' && paramSymbol != -1);
    
    return node;
}


Node *load_scene(const char *path, Camera **c, Script *scripts) {
    Game.buffers->collisionBuffer.length = 0;
    Game.buffers->lightingBuffer.length = 0;
    Game.buffers->collisionBuffer.index = 0;
    Game.buffers->lightingBuffer.index = 0;
    FILE * file = fopen(path, "r");

    if (!file) return NULL;

    Node *root;
    root = load_node(file, c, scripts, 0);
    if (!root) {
        fclose(file);
        return NULL;
    }
    root->parent = NULL;
    root::print(0);

    Game.buffers->collisionBuffer.collisionsShapes = realloc(Game.buffers->collisionBuffer.collisionsShapes, sizeof(Node *) * Game.buffers->collisionBuffer.length);
    // Check if the memory allocation was successful

    Game.buffers->lightingBuffer.lightings = realloc(Game.buffers->lightingBuffer.lightings, sizeof(Node *) * Game.buffers->lightingBuffer.length);
    // Check if the memory allocation was successful

    fclose(file);

    return root;

}