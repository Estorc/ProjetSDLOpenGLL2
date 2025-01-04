#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
#include "../../io/scene_loader.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_BODY;


void __class_method_body_is_body(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
bool * body = va_arg(args, bool *);
(void)this;
    (*body) = true;
}


void __class_method_body_apply_impulse(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
(void)this;
    // Do nothing
}
