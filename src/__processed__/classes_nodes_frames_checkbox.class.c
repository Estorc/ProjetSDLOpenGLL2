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
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CHECKBOX;
#line 14 "src/classes/nodes/frames/checkbox.class.c"
void __class_method_checkbox_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        this::init_frame();
}

    

#line 27 "src/classes/nodes/frames/checkbox.class.c"
void __class_method_checkbox_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        IGNORE(file);
        this->type = __type__;
        this::constructor();
        this::init_button();
        Frame *frame = (Frame *) this->object;
        frame->scale[0] = 48.0f;
        frame->scale[1] = 48.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';
}

#line 41 "src/classes/nodes/frames/checkbox.class.c"
void __class_method_checkbox_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
}

#line 46 "src/classes/nodes/frames/checkbox.class.c"
void __class_method_checkbox_is_checkbox(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = true;
}

#line 50 "src/classes/nodes/frames/checkbox.class.c"
void __class_method_checkbox_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        SUPER(free);
}
 
    
