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

class ImageFrame extends Frame {
    __containerType__ Node *

    void constructor() {
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        METHOD(this, init_frame);
        frame->imageFrame = malloc(sizeof(ImageFrame));
        POINTER_CHECK(frame->imageFrame);
        frame->flags &= ~FRAME_BACKGROUND;
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
        METHOD_TYPE(this, __type__, constructor);
        Frame *frame = (Frame *) this->object;
        if (file) {
            fscanf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c, %[^)])", 
                &frame->relPos[0],&frame->unit[0], 
                &frame->relPos[1],&frame->unit[1], 
                &frame->scale[0],&frame->unit[2], 
                &frame->scale[1],&frame->unit[3], 
                &frame->alignment[0], &frame->alignment[1],
                frame->imageFrame->path);
            frame->contentTexture = load_texture_from_path(frame->imageFrame->path, GL_SRGB_ALPHA, false);
            frame->flags |= FRAME_CONTENT;
        }
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
        ImageFrame *imageFrame = (ImageFrame *) frame->imageFrame;
        free(imageFrame);
        frame->flags &= ~FRAME_CONTENT; // Ensure the glTexture is not deleted
        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        SUPER(free);
    }
    
}