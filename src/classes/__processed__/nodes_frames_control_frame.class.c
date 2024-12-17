#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../storage/node.h"
#include "../../io/shader.h"
#include "../../render/render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../../window.h"
#include "../../gui/frame.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CONTROLFRAME;


void __class_method_controlframe_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    Frame *frame;
    frame = malloc(sizeof(Frame));
    POINTER_CHECK(frame);

    this->object = frame;
    this->type = __type__; 
    SUPER(initialize_node);
    METHOD(this, init_frame);
}


void __class_method_controlframe_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_controlframe_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    METHOD_TYPE(this, __type__, constructor);
    Frame *frame = (Frame *) this->object;
    if (file) {
        fscanf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c)", 
            &frame->relPos[0],&frame->unit[0], 
            &frame->relPos[1],&frame->unit[1], 
            &frame->scale[0],&frame->unit[2], 
            &frame->scale[1],&frame->unit[3], 
            &frame->alignment[0], &frame->alignment[1]);

        frame->flags |= FRAME_BACKGROUND;
    }
}


void __class_method_controlframe_render(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    Frame *frame = (Frame *) this->object;
    if (frame->flags & FRAME_NEEDS_REFRESH) METHOD(this, refresh);
}


void __class_method_controlframe_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Node * editor = va_arg(args, Node *);
va_end(args);
(void)this;
    Frame *frame = (Frame *) this->object;
    IGNORE(editor);
    fprintf(file, "%s", classManager.class_names[this->type]);

    fprintf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c)", 
        frame->relPos[0],frame->unit[0], 
        frame->relPos[1],frame->unit[1], 
        frame->scale[0],frame->unit[2], 
        frame->scale[1],frame->unit[3], 
        frame->alignment[0], frame->alignment[1]);
}
    

