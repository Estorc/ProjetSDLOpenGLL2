/**
 * @file
 * @brief This file defines the implementation of the control frame class.
 *
 * This source file contains the implementation details for the control frame class,
 * which is responsible for managing and rendering control frames within the application.
 * It includes functions for initializing, updating, and rendering control frames,
 * as well as handling user input and interactions.
 *
 * @author Loup Picault
 * @date October 6, 2023
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

class ControlFrame : public Frame {
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
        this->type = __type__;
        this::constructor();
        Frame *frame = (Frame *) this->object;
        char scroll;
        if (file) {
            fscanf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c%c)", 
                &frame->relPos[0],&frame->unit[0], 
                &frame->relPos[1],&frame->unit[1], 
                &frame->scale[0],&frame->unit[2], 
                &frame->scale[1],&frame->unit[3], 
                &frame->alignment[0], &frame->alignment[1],
                &scroll);

            frame->flags |= FRAME_BACKGROUND;
            if (scroll == 's') {
                frame->flags |= OVERFLOW_SCROLL;
            } else if (scroll == 'v') {
                frame->flags |= OVERFLOW_VISIBLE;
            }
        }
    }

    void render() {
        Frame *frame = (Frame *) this->object;
        if (frame->flags & FRAME_NEEDS_REFRESH) this::refresh();
    }

    void save(FILE *file, Node *editor) {
        Frame *frame = (Frame *) this->object;
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);

        fprintf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c%c)", 
            frame->relPos[0],frame->unit[0], 
            frame->relPos[1],frame->unit[1], 
            frame->scale[0],frame->unit[2], 
            frame->scale[1],frame->unit[3], 
            frame->alignment[0], frame->alignment[1],
            (frame->flags & OVERFLOW_SCROLL) ? 's' : (frame->flags & OVERFLOW_VISIBLE) ? 'v' : 'n');
    }
    
}