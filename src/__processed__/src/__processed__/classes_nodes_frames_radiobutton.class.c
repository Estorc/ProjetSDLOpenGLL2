#line 1 "src/__processed__/classes_nodes_frames_radiobutton.class.c"
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
#include "../io/input.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_RADIOBUTTON;
#line 14 "src/classes/nodes/frames/radiobutton.class.c"
void __class_method_radiobutton_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);
        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        call_method_0(METHOD(init_frame,this));
        call_method_0(METHOD(init_button,this));
        call_method_0(METHOD(init_radiobutton,this));
}

#line 26 "src/classes/nodes/frames/radiobutton.class.c"
void __class_method_radiobutton_init_radiobutton(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        frame->button->radiobutton = malloc(sizeof(RadioButton));
        RadioButton *radiobutton = frame->button->radiobutton;
        radiobutton->checked = NULL;
}

    

#line 35 "src/classes/nodes/frames/radiobutton.class.c"
void __class_method_radiobutton_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        IGNORE(file);
        this->type = __type__;
        call_method_0(METHOD(constructor,this));
        Frame *frame = (Frame *) this->object;
        frame->scale[0] = 48.0f;
        frame->scale[1] = 48.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';
}

#line 48 "src/classes/nodes/frames/radiobutton.class.c"
void __class_method_radiobutton_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
}

#line 53 "src/classes/nodes/frames/radiobutton.class.c"
void __class_method_radiobutton_is_radiobutton(void * __retValueVP__, va_list args) {bool * __retValueP__ = (bool *) __retValueVP__;Node * this = va_arg(args, Node *);(void)this;
        *__retValueP__ = true;return;
}

#line 57 "src/classes/nodes/frames/radiobutton.class.c"
void __class_method_radiobutton_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        Button *button = (Button *) frame->button;  
        RadioButton *radiobutton = button->radiobutton;
        free(radiobutton);
        SUPER(free);
}
 
    
