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

class Button : public Frame {
    __containerType__ Node *
    public:

    void constructor() {
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        METHOD(this, init_frame);
    }

    void init_button() {
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

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        IGNORE(file);
        METHOD_TYPE(this, __type__, constructor);
        METHOD(this, init_button);
    }

    void update() {
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

    void is_button(bool *result) {
        *result = true;
    }

    void save(FILE *file, Node *editor) {
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    void free() {
        Frame *frame = (Frame *) this->object;
        Button *button = (Button *) frame->button;
        free(button);
        SUPER(free);
    }
 
    
}