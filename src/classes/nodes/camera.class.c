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

    void update(vec3 *pos, vec3 *rot, vec3 *scale) {
        Camera *camera = (Camera *) this->object;

        this::update_global_position(pos, rot, scale);
        glm_vec3_negate_to(*pos, camera->pos);
        glm_vec3_negate_to(*rot, camera->rot);
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
                else if (editor) editor->attribute[5].node = this;
            }
        }
        this->type = __type__;
        this::constructor(cam);
    }

    void save(FILE *file, Node *editor) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        if (editor) fprintf(file, "(%d)", !!(editor->attribute[5].node == this));
        else fprintf(file, "(%d)", 0);
    }




    
}
