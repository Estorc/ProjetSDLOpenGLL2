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


void __class_method_body_is_body(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
bool * body = va_arg(args, bool *);
va_end(args);
(void)this;
    (*body) = true;
}


void __class_method_body_free(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    u8 *length;
    Node ***shapes;
    GET_FROM_BODY_NODE(this, length, length);
    GET_FROM_BODY_NODE(this, collisionsShapes, shapes);
    for (int j = 0; j < *length; j++) {
        free((*shapes)[j]);
    }
    free(*shapes);
    SUPER(free);
}


void __class_method_body_apply_impulse(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    // Do nothing
}
