#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_BOXCSHAPE;


void __class_method_boxcshape_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct BoxCollisionShape * boxCollisionShape = va_arg(args, struct BoxCollisionShape *);
va_end(args);
(void)this;
    this->object = boxCollisionShape;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_boxcshape_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_boxcshape_get_priority(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
int * priority = va_arg(args, int *);
va_end(args);
(void)this;
    *priority = 0;
}


void __class_method_boxcshape_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    BoxCollisionShape *boxCollisionShape;
    boxCollisionShape = malloc(sizeof(BoxCollisionShape));
    POINTER_CHECK(boxCollisionShape);
    METHOD_TYPE(this, __type__, constructor, boxCollisionShape);
}


void __class_method_boxcshape_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}








    

