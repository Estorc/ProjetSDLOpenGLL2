#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_FRAMEBUFFER;


void __class_method_framebuffer_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FrameBuffer * frameBuffer = va_arg(args, FrameBuffer *);
va_end(args);
(void)this;
    this->object = frameBuffer;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_framebuffer_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_framebuffer_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    FrameBuffer *msaa_framebuffer;
    msaa_framebuffer = malloc(sizeof(FrameBuffer));

    METHOD(this, constructor, msaa_framebuffer);
}


void __class_method_framebuffer_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}








    

