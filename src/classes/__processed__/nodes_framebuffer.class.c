#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
void __class_method_framebuffer_constructor(unsigned type, ...) {
unsigned __type__ = 11;
(void)__type__;
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
unsigned __type__ = 11;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_framebuffer_load(unsigned type, ...) {
unsigned __type__ = 11;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    FrameBuffer *msaa_framebuffer;
    msaa_framebuffer = malloc(sizeof(FrameBuffer));
    POINTER_CHECK(msaa_framebuffer);
    FrameBuffer *msaa_framebuffer_intermediate;
    msaa_framebuffer_intermediate = malloc(sizeof(FrameBuffer));
    POINTER_CHECK(msaa_framebuffer_intermediate);
    Filter *msaa_framebuffer_filter;
    msaa_framebuffer_filter = malloc(sizeof(Filter));
    POINTER_CHECK(msaa_framebuffer_filter);

    msaa_framebuffer_filter->beginScript = anti_aliasing_begin_script;
    msaa_framebuffer_filter->endScript = anti_aliasing_end_script;
    msaa_framebuffer_filter->flags = FILTER_DEFAULT_FLAGS;
    create_msaa_framebuffer(msaa_framebuffer, msaa_framebuffer_intermediate, &msaa_framebuffer_filter->texture);

    METHOD_TYPE(this, CLASS_TYPE_FILTER, constructor, msaa_framebuffer_filter);

    this->children = realloc(this->children, sizeof(Node *) * 2);
    POINTER_CHECK(this->children);

    Node *frameBufferMSAA;
    frameBufferMSAA = malloc(sizeof(Node));
    POINTER_CHECK(frameBufferMSAA);
    METHOD_TYPE(frameBufferMSAA, CLASS_TYPE_FRAMEBUFFER, constructor, msaa_framebuffer);
    add_child(this, frameBufferMSAA);

    Node *frameBufferMSAAIntermediate;
    frameBufferMSAAIntermediate = malloc(sizeof(Node));
    POINTER_CHECK(frameBufferMSAAIntermediate);
    METHOD_TYPE(frameBufferMSAAIntermediate, CLASS_TYPE_FRAMEBUFFER, constructor, msaa_framebuffer_intermediate);
    add_child(this, frameBufferMSAAIntermediate);
}

void __class_method_framebuffer_save(unsigned type, ...) {
unsigned __type__ = 11;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}

