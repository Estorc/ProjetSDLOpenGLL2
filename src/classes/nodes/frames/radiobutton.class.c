#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window.h"
#include "gui/frame.h"
#include "io/input.h"

class RadioButton extends Button {
    __containerType__ Node *

    void constructor() {
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

    void init_radiobutton() {
        Frame *frame = (Frame *) this->object;
        frame->button->radiobutton = malloc(sizeof(RadioButton));
        RadioButton *radiobutton = frame->button->radiobutton;
        radiobutton->checked = NULL;
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
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

    void save(FILE *file, Node *editor) {
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    void is_radiobutton(bool *result) {
        *result = true;
    }

    void free() {
        Frame *frame = (Frame *) this->object;
        Button *button = (Button *) frame->button;  
        RadioButton *radiobutton = button->radiobutton;
        free(radiobutton);
        SUPER(free);
    }
 
    
}