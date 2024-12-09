#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
void __class_method_texture_constructor(unsigned type, ...) {
unsigned __type__ = 18;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
TextureMap * texture = va_arg(args, TextureMap *);
va_end(args);
(void)this;
    this->object = texture;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_texture_cast(unsigned type, ...) {
unsigned __type__ = 18;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_texture_load(unsigned type, ...) {
unsigned __type__ = 18;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;

}

void __class_method_texture_save(unsigned type, ...) {
unsigned __type__ = 18;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;

}

