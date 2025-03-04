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

    void constructor() {
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);
        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        this::init_frame();
        this::init_slider();
    }

    void init_slider() {
        Frame *frame = (Frame *) this->object;
        frame->slider = malloc(sizeof(Slider));
        Slider *slider = frame->slider;
        slider->value = slider->max;
        slider->min = 0.0f;
        slider->max = 100.0f;
    }

    

    void load(FILE *file) {
        UNUSED(file);
        this->type = __type__;
        this::constructor();
        Frame *frame = (Frame *) this->object;
        frame->scale[0] = 24.0f;
        frame->scale[1] = 24.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';
        if (file) {
            static char text[2048]; // In static to prevent stack overflow
            fscanf(file, "(%g,%g,%g)", 
            &frame->slider->min, &frame->slider->max, &frame->slider->value);
            format_escaped_newlines(text);
            frame->label->text = malloc(strlen(text) + 1);
            POINTER_CHECK(frame->label->text);
            strcpy(frame->label->text, text);
        }
    }

    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        Frame *frame = (Frame *) this->object;
        fprintf(file, "%s(%g,%g,%g)", classManager.class_names[this->type],
        frame->slider->min, frame->slider->max, frame->slider->value);
    }

    bool is_radiobutton() {
        return true;
    }

    void update() {
        Frame * frame = this->object;
        Frame * parentFrame = this->parent->object;
        this->pos[0] = -this->parent->globalScale[0] + this->parent->globalScale[0] * 2.0f * ((SDL_GetTicks()%2000)/2000.0f);
        frame->absPos[0] = parentFrame->absPos[0] + parentFrame->size[0] * ((SDL_GetTicks()%2000)/2000.0f) - frame->size[0]/2;
        SUPER(update);
    }

    void free() {
        SUPER(free);
    }
 
    
}