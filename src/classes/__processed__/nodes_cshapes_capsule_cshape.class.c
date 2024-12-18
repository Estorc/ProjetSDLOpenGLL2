#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CAPSULECSHAPE;


void __class_method_capsulecshape_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct CapsuleCollisionShape * capsuleCollisionShape = va_arg(args, struct CapsuleCollisionShape *);
va_end(args);
(void)this;
    this->object = capsuleCollisionShape;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_capsulecshape_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_capsulecshape_get_priority(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
int * priority = va_arg(args, int *);
va_end(args);
(void)this;
    *priority = 3;
}


void __class_method_capsulecshape_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    CapsuleCollisionShape *capsuleCollisionShape;
    capsuleCollisionShape = malloc(sizeof(CapsuleCollisionShape));
    POINTER_CHECK(capsuleCollisionShape);
    METHOD_TYPE(this, __type__, constructor, capsuleCollisionShape);
}


void __class_method_capsulecshape_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}








    

