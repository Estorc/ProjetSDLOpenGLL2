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

class RadioButton : public Button {
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
        this::init_radiobutton();
        frame->scale[0] = 48.0f;
        frame->scale[1] = 48.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';
    }

    void init_radiobutton() {
        Frame *frame = (Frame *) this->object;
        frame->radiobutton = malloc(sizeof(RadioButton));
        RadioButton *radiobutton = frame->radiobutton;
        radiobutton->checked = NULL;
    }

    

    void load() {
        this::constructor();
    }

    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    bool is_radiobutton() {
        return true;
    }

    void free() {
        SUPER(free);
    }
 
    
}