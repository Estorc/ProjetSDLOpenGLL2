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
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_STATICBODY;


void __class_method_staticbody_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct StaticBody * staticBody = va_arg(args, struct StaticBody *);
va_end(args);
(void)this;
    this->object = staticBody;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_staticbody_get_collisions_shapes(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
Node *** shapes = va_arg(args, Node ***);
u8 * length = va_arg(args, u8 *);
va_end(args);
(void)this;
    StaticBody *staticBody = (StaticBody *) this->object;
    *length = staticBody->length;
    *shapes = staticBody->collisionsShapes;
}


void __class_method_staticbody_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Camera ** c = va_arg(args, Camera **);
Script * scripts = va_arg(args, Script *);
Node * editor = va_arg(args, Node *);
va_end(args);
(void)this;
    StaticBody *staticBody;
    staticBody = malloc(sizeof(StaticBody));
    staticBody->length = 0;
    int children_count = 0;
    POINTER_CHECK(staticBody);
    if (file)
        fscanf(file,"(%d)\n", &children_count);
    METHOD_TYPE(this, __type__, constructor, staticBody);

    staticBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
    buffers.collisionBuffer.length += children_count;
    POINTER_CHECK(staticBody->collisionsShapes);
    
    for (int i = 0; i < children_count; i++) {
        Node *child = load_node(file, c, scripts, editor);
        staticBody->collisionsShapes[staticBody->length++] = child;
        child->parent = this;
    }
}


void __class_method_staticbody_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    StaticBody *staticBody = (StaticBody*) this->object;
    u8 collisionsLength = staticBody->length;
    fprintf(file, "(%d)", collisionsLength);
}
