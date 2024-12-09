#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
void __class_method_cshape_is_cshape(unsigned type, ...) {
unsigned __type__ = 7;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
bool * cshape = va_arg(args, bool *);
va_end(args);
(void)this;
    (*cshape) = true;
}

