#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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
        METHOD(this, init_frame);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        METHOD_TYPE(this, __type__, constructor);
        Frame *frame = (Frame *) this->object;
        if (file) {
            fscanf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c)", 
                &frame->relPos[0],&frame->unit[0], 
                &frame->relPos[1],&frame->unit[1], 
                &frame->scale[0],&frame->unit[2], 
                &frame->scale[1],&frame->unit[3], 
                &frame->alignment[0], &frame->alignment[1]);

            frame->flags |= FRAME_BACKGROUND;
        }
    }

    void render() {
        Frame *frame = (Frame *) this->object;
        if (frame->flags & FRAME_NEEDS_REFRESH) METHOD(this, refresh);
    }

    void save(FILE *file, Node *editor) {
        Frame *frame = (Frame *) this->object;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);

        fprintf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c)", 
            frame->relPos[0],frame->unit[0], 
            frame->relPos[1],frame->unit[1], 
            frame->scale[0],frame->unit[2], 
            frame->scale[1],frame->unit[3], 
            frame->alignment[0], frame->alignment[1]);
    }
    
}