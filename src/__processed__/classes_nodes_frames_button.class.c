#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../storage/node.h"
#include "../io/shader.h"
#include "../render/render.h"
#include "../window.h"
#include "../gui/frame.h"
#include "../io/input.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_BUTTON;
#line 14 "src/classes/nodes/frames/button.class.c"
void __class_method_button_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        this::init_frame();
}

#line 25 "src/classes/nodes/frames/button.class.c"
void __class_method_button_init_button(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        frame->button = malloc(sizeof(Button));
        POINTER_CHECK(frame->button);
        frame->button->checked = NULL;
        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->scale[0] = 100.0f;
        frame->scale[1] = 100.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';
        frame->unit[2] = '%';
        frame->unit[3] = '%';
}

    

#line 42 "src/classes/nodes/frames/button.class.c"
void __class_method_button_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        IGNORE(file);
        this->type = __type__;
        this::constructor();
        this::init_button();
}

#line 49 "src/classes/nodes/frames/button.class.c"
void __class_method_button_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        Button *button = (Button *) frame->button;
        float x,y,w,h;
        x = frame->absPos[0];
        y = frame->absPos[1];
        w = frame->size[0];
        h = frame->size[1];
        if (button->state == BUTTON_STATE_PRESSED) {
            if (input.mouse.released_button == SDL_BUTTON_LEFT) {
                button->state = BUTTON_STATE_NORMAL;
                if (button->checked &&
                    input.mouse.x > x &&
                    input.mouse.x < x+w &&
                    input.mouse.y > y &&
                    input.mouse.y < y+h)
                    *button->checked = !(*button->checked);
            }
        } else {
            if (input.mouse.x > x &&
                input.mouse.x < x+w &&
                input.mouse.y > y &&
                input.mouse.y < y+h) {

                button->state = BUTTON_STATE_HOVERED;

                if (input.mouse.pressed_button == SDL_BUTTON_LEFT)
                    button->state = BUTTON_STATE_PRESSED;

            } else {
                button->state = BUTTON_STATE_NORMAL;
            }
        }
        if (window.resized) frame->flags |= FRAME_NEEDS_REFRESH;
}

#line 85 "src/classes/nodes/frames/button.class.c"
void __class_method_button_is_button(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = true;
}

#line 89 "src/classes/nodes/frames/button.class.c"
void __class_method_button_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
}

#line 94 "src/classes/nodes/frames/button.class.c"
void __class_method_button_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        Button *button = (Button *) frame->button;
        free(button);
        SUPER(free);
}
 
    
