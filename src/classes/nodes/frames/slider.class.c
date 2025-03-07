/**
 * @file radiobutton.class.c
 * @brief Implementation of the RadioButton class.
 *
 * This source file contains the implementation of the RadioButton class, which is used to create and manage radio button UI elements in the application. The RadioButton class provides functionality to handle user interactions, rendering, and state management of radio buttons.
 *
 * This file is part of the larger project that integrates SDL and OpenGL for rendering graphical user interfaces.
 *
 * @author Loup Picault
 * @date 2023-10-20
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

class Slider : public Button {
    __containerType__ Node *
    public:

    void constructor(double min, double max) {
        this->type = __type__; 
        
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);
        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();
        this::init_slider();

        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';

        frame->scale[0] = 24.0f;
        frame->scale[1] = 24.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';

        frame->slider->min = min;
        frame->slider->max = max;
    }

    void init_slider() {
        Frame *frame = (Frame *) this->object;
        frame->slider = malloc(sizeof(Slider));
        Slider *slider = frame->slider;
        slider->checked = NULL;
        slider->value = NULL;
        slider->min = 0.0f;
        slider->max = 1.0f;
    }

    

    void load(FILE *file) {
        float min, max;
        if (file) {
            fscanf(file, "(%g,%g)", 
            &min, &max);
        }
        this::constructor(min, max);
    }

    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        Frame *frame = (Frame *) this->object;
        fprintf(file, "%s(%g,%g)", classManager.class_names[this->type],
        frame->slider->min, frame->slider->max);
    }

    bool is_radiobutton() {
        return true;
    }

    void update() {
        Frame * frame = this->object;
        Frame * parentFrame = this->parent->object;

        SUPER(update);
        if (frame->slider->state == BUTTON_STATE_PRESSED) {
            float w = parentFrame->size[0];
            float value = (Game.input->mouse.x - parentFrame->absPos[0]) / w;
            *frame->slider->value = frame->slider->min + (frame->slider->max - frame->slider->min) * value;
            *frame->slider->value = CLAMP(frame->slider->min, *frame->slider->value, frame->slider->max);
        }
        float percent = *frame->slider->value / frame->slider->max;
        if (frame->slider->state != BUTTON_STATE_PRESSED) {
            frame->absPos[0] = parentFrame->absPos[0] + parentFrame->size[0] * percent - frame->size[0]/2;
        }
        this->pos[0] = -this->parent->globalScale[0] + this->parent->globalScale[0] * 2.0f * percent;
    }

    void free() {
        SUPER(free);
    }
 
    
}