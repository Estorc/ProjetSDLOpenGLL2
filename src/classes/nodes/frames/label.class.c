#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include "window.h"
#include "gui/frame.h"

class Label : public Frame {
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
        frame->label = malloc(sizeof(Label));
        POINTER_CHECK(frame->label);
        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->scale[0] = 100.0f;
        frame->scale[1] = 100.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';
        frame->unit[2] = '%';
        frame->unit[3] = '%';
        frame->flags |= FRAME_CONTENT;
        glGenTextures(1, &frame->contentTexture);
    }

    

    void load(FILE *file) {
        this->type = __type__;
        this::constructor();
        Frame *frame = (Frame *) this->object;
        if (file) {
            fscanf(file, "(%[^,],%c%c)", 
            frame->label->text, &frame->alignment[0], &frame->alignment[1]);
        }
    }


    void refresh() {
        SUPER(refresh);
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;

        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        frame->contentSurface = SDL_CreateRGBSurface(0,frame->size[0],frame->size[1],32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (!frame->contentSurface) {
            printf("Error creating surface: %s\n", SDL_GetError());
        }
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