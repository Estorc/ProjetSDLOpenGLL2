#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
void __class_method_spherecshape_constructor(unsigned type, ...) {
unsigned __type__ = 9;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct SphereCollisionShape * sphereCollisionShape = va_arg(args, struct SphereCollisionShape *);
va_end(args);
(void)this;
    this->object = sphereCollisionShape;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_spherecshape_cast(unsigned type, ...) {
unsigned __type__ = 9;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_spherecshape_get_priority(unsigned type, ...) {
unsigned __type__ = 9;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
int * priority = va_arg(args, int *);
va_end(args);
(void)this;
    *priority = 1;
}

void __class_method_spherecshape_load(unsigned type, ...) {
unsigned __type__ = 9;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    SphereCollisionShape *sphereCollisionShape;
    sphereCollisionShape = malloc(sizeof(SphereCollisionShape));
    POINTER_CHECK(sphereCollisionShape);
    METHOD_TYPE(this, CLASS_TYPE_SPHERECSHAPE, constructor, sphereCollisionShape);
}

void __class_method_spherecshape_save(unsigned type, ...) {
unsigned __type__ = 9;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}

