#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_NODE;


void __class_method_node_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    this->object = NULL;
    this->type = __type__;
    METHOD(this, initialize_node);
}


void __class_method_node_initialize_node(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    this->flags = NODE_DEFAULT_FLAGS;
    this->length = 0;
    this->children = NULL;
    this->script = NULL;
    this->params = NULL;
    this->params_count = 0;
    this->shader = 0;
    Vec3fZero(this->pos);
    Vec3fZero(this->rot);
    Vec3fOne(this->scale);
}


void __class_method_node_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_node_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    METHOD_TYPE(this, __type__, constructor);
}


void __class_method_node_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}


void __class_method_node_render(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    //
}


void __class_method_node_update(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    //
}


void __class_method_node_free(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    for (int i = 0; i < this->length; i++) {
        METHOD(this->children[i], free);
    }
    free(this->object);
    free(this->params);
    free(this->children);
    free(this);
}


void __class_method_node_is_cshape(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
bool * cshape = va_arg(args, bool *);
va_end(args);
(void)this;
    *cshape = false;
}


void __class_method_node_is_body(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
bool * body = va_arg(args, bool *);
va_end(args);
(void)this;
    *body = false;
}


void __class_method_node_is_area(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
bool * area = va_arg(args, bool *);
va_end(args);
(void)this;
    *area = false;
}


void __class_method_node_is_gui_element(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
bool * result = va_arg(args, bool *);
va_end(args);
(void)this;
    *result = false;
}
