#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CAMERA;


void __class_method_camera_constructor(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
struct Camera * camera = va_arg(args, struct Camera *);
(void)this;
    this->object = camera;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_camera_cast(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
(void)this;
    IGNORE(data);
}


void __class_method_camera_load(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Camera ** c = va_arg(args, Camera **);
Script * scripts = va_arg(args, Script *);
Node * editor = va_arg(args, Node *);
(void)this;
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


void __class_method_camera_save(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Node * editor = va_arg(args, Node *);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    if (editor) fprintf(file, "(%d)", !!(editor->params[5].node == this));
    else fprintf(file, "(%d)", 0);
}








    

