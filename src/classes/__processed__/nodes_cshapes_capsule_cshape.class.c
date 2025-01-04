#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CAPSULECSHAPE;


void __class_method_capsulecshape_constructor(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
struct CapsuleCollisionShape * capsuleCollisionShape = va_arg(args, struct CapsuleCollisionShape *);
(void)this;
    this->object = capsuleCollisionShape;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_capsulecshape_cast(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
(void)this;
    IGNORE(data);
}


void __class_method_capsulecshape_get_priority(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
int * priority = va_arg(args, int *);
(void)this;
    *priority = 3;
}


void __class_method_capsulecshape_load(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
(void)this;
    CapsuleCollisionShape *capsuleCollisionShape;
    capsuleCollisionShape = malloc(sizeof(CapsuleCollisionShape));
    POINTER_CHECK(capsuleCollisionShape);
    METHOD_TYPE(this, __type__, constructor, capsuleCollisionShape);
}


void __class_method_capsulecshape_save(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}








    

