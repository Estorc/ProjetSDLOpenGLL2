#include <stdarg.h>
#include "../types.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../storage/node.h"
#include "../io/shader.h"
#include "../render/render.h"
#include "../window.h"
#include "../gui/frame.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CONTROLFRAME;
#line 13 "src/classes/nodes/frames/control_frame.class.c"
void __class_method_controlframe_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        this::init_frame();
}

    

#line 26 "src/classes/nodes/frames/control_frame.class.c"
void __class_method_controlframe_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        this->type = __type__;
        this::constructor();
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

#line 42 "src/classes/nodes/frames/control_frame.class.c"
void __class_method_controlframe_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        if (frame->flags & FRAME_NEEDS_REFRESH) this::refresh();
}

#line 47 "src/classes/nodes/frames/control_frame.class.c"
void __class_method_controlframe_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
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
    
