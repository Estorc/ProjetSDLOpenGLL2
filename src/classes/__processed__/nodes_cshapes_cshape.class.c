#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CSHAPE;


void __class_method_cshape_is_cshape(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
bool * cshape = va_arg(args, bool *);
(void)this;
    (*cshape) = true;
}
    

