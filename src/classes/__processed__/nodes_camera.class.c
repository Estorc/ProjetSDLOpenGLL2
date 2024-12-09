#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
void __class_method_camera_constructor(unsigned type, ...) {
unsigned __type__ = 5;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct Camera * camera = va_arg(args, struct Camera *);
va_end(args);
(void)this;
    this->object = camera;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_camera_cast(unsigned type, ...) {
unsigned __type__ = 5;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_camera_load(unsigned type, ...) {
unsigned __type__ = 5;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Camera ** c = va_arg(args, Camera **);
Script * scripts = va_arg(args, Script *);
Node * editor = va_arg(args, Node *);
va_end(args);
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
    METHOD_TYPE(this, CLASS_TYPE_CAMERA, constructor, cam);
}

void __class_method_camera_save(unsigned type, ...) {
unsigned __type__ = 5;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Node * editor = va_arg(args, Node *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    if (editor) fprintf(file, "(%d)", !!(editor->params[5].node == this));
    else fprintf(file, "(%d)", 0);
}

