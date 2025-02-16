/**
 * @file checkbox.class.c
 * @brief Implementation of the Checkbox class for the GUI framework.
 *
 * This source file contains the implementation of the Checkbox class, which is a part of the GUI framework
 * used in the project. The Checkbox class provides functionality to create and manage checkbox elements
 * within the graphical user interface. It handles rendering, state management, and user interaction
 * for checkboxes.
 *
 * This file is used for creating interactive checkbox components that can be used in various parts of the
 * application to allow users to make binary choices.
 *
 * @author Loup Picault
 * @date 2023-10-25
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

class CheckBox : public Button {
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
    }

    

    void load(FILE *file) {
        UNUSED(file);
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

    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    void is_checkbox(bool *result) {
        *result = true;
    }

    void free() {
        SUPER(free);
    }
 
    
}