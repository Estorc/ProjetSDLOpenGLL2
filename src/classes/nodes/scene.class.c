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

    void constructor(const char * path) {
        FILE * file = fopen(path, "r");
        if (!file) return;
        Node *child = load_node(file, &Game.camera, Game.scripts, NULL);
        if (!child) return;

        memcpy(this, child, sizeof(Node));
        for (int i = 0; i < this->length; i++) {
            this->children[i]->parent = this;
        }
        free(child);
    }
    

    void load(FILE *file) {
        char path[256];
        if (file) {
            fscanf(file,"(%[^)])\n", 
                path);
            this::constructor(path);
        }
    }
    
}
