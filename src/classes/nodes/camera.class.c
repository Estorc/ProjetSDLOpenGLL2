#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"

class Camera @promote extends Node {
    __containerType__ Node *

    void constructor(struct Camera *camera) {
        this->object = camera;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        IGNORE(scripts);
        Camera *cam;
        cam = malloc(sizeof(Camera));
        POINTER_CHECK(cam);
        init_camera(cam);
        if (file) {
            int active_camera;
            fscanf(file,"(%d)", &active_camera);
            if (active_camera) {
                if (c) *c = cam;
                else if (editor) editor->params[5].node = this;
            }
        }
        METHOD_TYPE(this, CLASS_TYPE_CAMERA, constructor, cam);
    }

    void save(FILE *file, Node *editor) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        if (editor) fprintf(file, "(%d)", !!(editor->params[5].node == this));
        else fprintf(file, "(%d)", 0);
    }




    
}
