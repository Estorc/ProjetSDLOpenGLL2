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
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_PHYSICALNODE;


void __class_method_physicalnode_free(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
(void)this;
    u8 length;
    Node **shapes;
    METHOD(this, get_collisions_shapes, &shapes, &length);
    for (int j = 0; j < length; j++) {
        free(shapes[j]);
    }
    free(shapes);
    SUPER(free);
}
