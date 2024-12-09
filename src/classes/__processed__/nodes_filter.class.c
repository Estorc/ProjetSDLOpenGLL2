#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
void __class_method_filter_constructor(unsigned type, ...) {
unsigned __type__ = 10;
(void)__type__;
va_list args;
va_start(args, type);
Node *  this = va_arg(args, Node * );
struct Filter * filter = va_arg(args, struct Filter *);
va_end(args);
(void)this;
    this->object = filter;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_filter_cast(unsigned type, ...) {
unsigned __type__ = 10;
(void)__type__;
va_list args;
va_start(args, type);
Node *  this = va_arg(args, Node * );
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_filter_load(unsigned type, ...) {
unsigned __type__ = 10;
(void)__type__;
va_list args;
va_start(args, type);
Node *  this = va_arg(args, Node * );
va_end(args);
(void)this;

}

void __class_method_filter_save(unsigned type, ...) {
unsigned __type__ = 10;
(void)__type__;
va_list args;
va_start(args, type);
Node *  this = va_arg(args, Node * );
va_end(args);
(void)this;

}

