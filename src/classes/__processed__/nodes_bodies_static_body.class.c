#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
#include "../../io/scene_loader.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
void __class_method_staticbody_constructor(unsigned type, ...) {
unsigned __type__ = 4;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct StaticBody * staticBody = va_arg(args, struct StaticBody *);
va_end(args);
(void)this;
    this->object = staticBody;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_staticbody_cast(unsigned type, ...) {
unsigned __type__ = 4;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_staticbody_load(unsigned type, ...) {
unsigned __type__ = 4;
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
    StaticBody *staticBody;
    staticBody = malloc(sizeof(StaticBody));
    staticBody->length = 0;
    int children_count = 0;
    POINTER_CHECK(staticBody);
    if (file)
        fscanf(file,"(%d)\n", &children_count);
    METHOD_TYPE(this, CLASS_TYPE_STATICBODY, constructor, staticBody);

    staticBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
    buffers.collisionBuffer.length += children_count;
    POINTER_CHECK(staticBody->collisionsShapes);
    
    for (int i = 0; i < children_count; i++) {
        Node *child = load_node(file, c, scripts, editor);
        staticBody->collisionsShapes[staticBody->length++] = child;
        child->parent = this;
    }
}

void __class_method_staticbody_save(unsigned type, ...) {
unsigned __type__ = 4;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    StaticBody *staticBody = (StaticBody*) this->object;
    u8 collisionsLength = staticBody->length;
    fprintf(file, "(%d)", collisionsLength);
}

