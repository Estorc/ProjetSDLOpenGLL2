/**
 * @file button.class.c
 * @brief This file defines the Button class and its associated methods.
 *
 * This source file contains the implementation of the Button class, which is used
 * to create and manage button elements within the application. The Button class
 * provides functionalities such as rendering the button, handling user interactions,
 * and triggering events when the button is clicked.
 *
 * The Button class is a crucial component for creating interactive user interfaces
 * in the application, allowing users to perform actions through button clicks.
 *
 * @author Loup Picault
 * @date 2023-10-31
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include "window.h"
#include "gui/frame.h"
#include "io/input.h"

class Button : public Frame {
    __containerType__ Node *
    public:

    void constructor() {
        this->type = __type__; 

        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();
        this::init_button();
    }

    void init_button() {
        Frame *frame = (Frame *) this->object;
        frame->button = malloc(sizeof(Button));
        POINTER_CHECK(frame->button);
        frame->button->checked = NULL;
        frame->button->state = BUTTON_STATE_NORMAL;
        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->scale[0] = 100.0f;
        frame->scale[1] = 100.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';
        frame->unit[2] = '%';
        frame->unit[3] = '%';
    }

    

    void load() {
        this::constructor();
    }

    void update() {
        Frame *frame = (Frame *) this->object;
        Button *button = (Button *) frame->button;
        Mouse *mouse = &Game.input->mouse;
        if (Game.renderTarget) {
            mouse = &Game.renderTarget->mouse;
        }
        float x,y,w,h;
        x = frame->absPos[0];
        y = frame->absPos[1];
        w = frame->size[0];
        h = frame->size[1];
        if (button->state == BUTTON_STATE_PRESSED) {
            if (mouse->active_button != SDL_BUTTON_LEFT) {
                button->state = BUTTON_STATE_NORMAL;
                if (mouse->x > x &&
                    mouse->x < x+w &&
                    mouse->y > y &&
                    mouse->y < y+h &&
                    mouse->x > frame->overflow[0] &&
                    mouse->x < frame->overflow[2] &&
                    mouse->y > frame->overflow[1] &&
                    mouse->y < frame->overflow[3]) {
                    if (button->checked) *button->checked = !(*button->checked);
                    this::emit_signal(SIGNAL_BUTTON_CLICKED);
                }
            }
        } else {
            if (mouse->x > x &&
                mouse->x < x+w &&
                mouse->y > y &&
                mouse->y < y+h &&
                mouse->x > frame->overflow[0] &&
                mouse->x < frame->overflow[2] &&
                mouse->y > frame->overflow[1] &&
                mouse->y < frame->overflow[3]) {

                if (button->state != BUTTON_STATE_HOVERED) {
                    this::emit_signal(SIGNAL_BUTTON_HOVERED);
                    button->state = BUTTON_STATE_HOVERED;
                }

                if (mouse->pressed_button == SDL_BUTTON_LEFT)
                    button->state = BUTTON_STATE_PRESSED;

            } else {
                button->state = BUTTON_STATE_NORMAL;
            }
        }
    }

    void is_button(bool *result) {
        *result = true;
    }

    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    void free() {
        Frame *frame = (Frame *) this->object;
        free(frame->button);
        SUPER(free);
    }
 
    
}