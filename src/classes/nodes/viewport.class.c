#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"

class Viewport @promote extends Node {
    __containerType__ Node *

    void constructor(struct Viewport *viewport) {
        this->object = viewport;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load() {
        Viewport *viewport;
        viewport = malloc(sizeof(Viewport));

        Mesh *mesh;
        mesh = malloc(sizeof(Mesh));
        POINTER_CHECK(mesh);
        create_screen_plane(mesh);
        Node *screenPlaneNode;
        screenPlaneNode = malloc(sizeof(Node));
        METHOD_TYPE(screenPlaneNode, CLASS_TYPE_MESH, constructor, mesh);
        viewport->screenPlane = screenPlaneNode;
        
        METHOD_TYPE(this, CLASS_TYPE_VIEWPORT, constructor, viewport);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    void free() {
        Viewport *viewport = (Viewport *) this->object;
        if (viewport->scene) {
            METHOD(viewport->scene, free);
        }
        SUPER(free);
    }
}
