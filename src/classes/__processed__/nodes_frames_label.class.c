#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../storage/node.h"
#include "../../io/shader.h"
#include "../../render/render.h"
#include <SDL2/SDL.h>
#include <limits.h>
#include <SDL2/SDL_ttf.h>
#include "../../window.h"
#include "../../gui/frame.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_LABEL;


void __class_method_label_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    Frame *frame;
    frame = malloc(sizeof(Frame));
    POINTER_CHECK(frame);

    this->object = frame;
    this->type = __type__; 
    SUPER(initialize_node);
    METHOD(this, init_frame);
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


void __class_method_label_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_label_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    METHOD_TYPE(this, __type__, constructor);
    Frame *frame = (Frame *) this->object;
    if (file) {
        fscanf(file, "(%[^,],%c%c)", 
        frame->label->text, &frame->alignment[0], &frame->alignment[1]);
    }
}




void __class_method_label_refresh(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
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




void __class_method_label_render(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
mat4 * modelMatrix = va_arg(args, mat4 *);
Shader  activeShader = va_arg(args, Shader );
WorldShaders * shaders = va_arg(args, WorldShaders *);
va_end(args);
(void)this;
    SUPER(render, modelMatrix, activeShader, shaders);
}


void __class_method_label_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Node * editor = va_arg(args, Node *);
va_end(args);
(void)this;
    Frame *frame = (Frame *) this->object;
    IGNORE(editor);
    fprintf(file, "%s", classManager.class_names[this->type]);
    fprintf(file, "(%s,%c%c)", 
    frame->label->text, frame->alignment[0], frame->alignment[1]);
}
 

void __class_method_label_free(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    Frame *frame = (Frame *) this->object;
    Label *label = (Label *) frame->label;
    free(label);
    SUPER(free);
}
    

