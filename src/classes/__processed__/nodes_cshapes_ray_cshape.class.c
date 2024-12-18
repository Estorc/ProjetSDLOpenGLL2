#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_RAYCSHAPE;


void __class_method_raycshape_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct RayCollisionShape * rayCollisionShape = va_arg(args, struct RayCollisionShape *);
va_end(args);
(void)this;
    this->object = rayCollisionShape;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_raycshape_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_raycshape_get_priority(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
int * priority = va_arg(args, int *);
va_end(args);
(void)this;
    *priority = 5;
}


void __class_method_raycshape_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    RayCollisionShape *rayCollisionShape;
    rayCollisionShape = malloc(sizeof(RayCollisionShape));
    POINTER_CHECK(rayCollisionShape);
    METHOD_TYPE(this, __type__, constructor, rayCollisionShape);
}


void __class_method_raycshape_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}








    

