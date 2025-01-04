#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Camera : public Node {
    __containerType__ Node *
    public:

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
        METHOD_TYPE(this, __type__, constructor, cam);
    }

    void save(FILE *file, Node *editor) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        if (editor) fprintf(file, "(%d)", !!(editor->params[5].node == this));
        else fprintf(file, "(%d)", 0);
    }




    
}
