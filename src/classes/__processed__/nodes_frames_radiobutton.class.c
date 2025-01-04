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
#include "../../io/input.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_RADIOBUTTON;


void __class_method_radiobutton_constructor(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
(void)this;
    Frame *frame;
    frame = malloc(sizeof(Frame));
    POINTER_CHECK(frame);
    this->object = frame;
    this->type = __type__; 
    SUPER(initialize_node);
    METHOD(this, init_frame);
    METHOD(this, init_button);
    METHOD(this, init_radiobutton);
}


void __class_method_radiobutton_init_radiobutton(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
(void)this;
    Frame *frame = (Frame *) this->object;
    frame->button->radiobutton = malloc(sizeof(RadioButton));
    RadioButton *radiobutton = frame->button->radiobutton;
    radiobutton->checked = NULL;
}


void __class_method_radiobutton_cast(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
(void)this;
    IGNORE(data);
}


void __class_method_radiobutton_load(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
(void)this;
    IGNORE(file);
    METHOD_TYPE(this, __type__, constructor);
    Frame *frame = (Frame *) this->object;
    frame->scale[0] = 48.0f;
    frame->scale[1] = 48.0f;
    frame->unit[2] = 'p';
    frame->unit[3] = 'p';
    frame->alignment[0] = 'c';
    frame->alignment[1] = 'c';
}


void __class_method_radiobutton_save(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Node * editor = va_arg(args, Node *);
(void)this;
    IGNORE(editor);
    fprintf(file, "%s", classManager.class_names[this->type]);
}


void __class_method_radiobutton_is_radiobutton(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
bool * result = va_arg(args, bool *);
(void)this;
    *result = true;
}


void __class_method_radiobutton_free(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
(void)this;
    Frame *frame = (Frame *) this->object;
    Button *button = (Button *) frame->button;  
    RadioButton *radiobutton = button->radiobutton;
    free(radiobutton);
    SUPER(free);
}
 

    

