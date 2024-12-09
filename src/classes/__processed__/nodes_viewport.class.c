#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
void __class_method_viewport_constructor(unsigned type, ...) {
unsigned __type__ = 20;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct Viewport * viewport = va_arg(args, struct Viewport *);
va_end(args);
(void)this;
    this->object = viewport;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_viewport_cast(unsigned type, ...) {
unsigned __type__ = 20;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_viewport_load(unsigned type, ...) {
unsigned __type__ = 20;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
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

void __class_method_viewport_save(unsigned type, ...) {
unsigned __type__ = 20;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}

void __class_method_viewport_free(unsigned type, ...) {
unsigned __type__ = 20;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    Viewport *viewport = (Viewport *) this->object;
    if (viewport->scene) {
        METHOD(viewport->scene, free);
    }
    SUPER(free);
}

