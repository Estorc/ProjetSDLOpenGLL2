#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include <SDL2/SDL.h>
#include <limits.h>
#include <SDL2/SDL_ttf.h>
#include "window.h"
#include "gui/frame.h"

class Label @promote extends Frame {
    __containerType__ Node *

    void constructor() {
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        METHOD(this, init_frame);
        frame->label = malloc(sizeof(Label));
        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->scale[0] = 100.0f;
        frame->scale[1] = 100.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';
        frame->unit[2] = '%';
        frame->unit[3] = '%';
        frame->flags |= FRAME_CONTENT;
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        METHOD_TYPE(this, __type__, constructor);
        Frame *frame = (Frame *) this->object;
        POINTER_CHECK(frame->label);
        if (file) {
            fscanf(file, "(%[^,],%c%c)", 
            frame->label->text, &frame->alignment[0], &frame->alignment[1]);
        }
    }


    void refresh() {
        SUPER(refresh);
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;

        if (label->text[0]) draw_text(frame->contentSurface, 0, 0, label->text, frame->theme->font.font, frame->theme->textColor, frame->alignment, -1);
        SUPER(refreshContent);
    }


    void render(mat4 *modelMatrix, Shader activeShader, WorldShaders *shaders) {
        SUPER(render, modelMatrix, activeShader, shaders);
    }

    void save(FILE *file, Node *editor) {
        Frame *frame = (Frame *) this->object;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
        fprintf(file, "(%s,%c%c)", 
        frame->label->text, frame->alignment[0], frame->alignment[1]);
    }
 
    void free() {
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;
        free(label);
        SUPER(free);
    }
    
}