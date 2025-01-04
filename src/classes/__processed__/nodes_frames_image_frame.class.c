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
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_IMAGEFRAME;


void __class_method_imageframe_constructor(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
(void)this;
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


void __class_method_imageframe_cast(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
(void)this;
    IGNORE(data);
}


void __class_method_imageframe_load(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
(void)this;
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




void __class_method_imageframe_render(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
mat4 * modelMatrix = va_arg(args, mat4 *);
Shader  activeShader = va_arg(args, Shader );
WorldShaders * shaders = va_arg(args, WorldShaders *);
(void)this;
    SUPER(render, modelMatrix, activeShader, shaders);
}


void __class_method_imageframe_save(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Node * editor = va_arg(args, Node *);
(void)this;
    Frame *frame = (Frame *) this->object;
    IGNORE(editor);
    fprintf(file, "%s", classManager.class_names[this->type]);
    fprintf(file, "(%s,%c%c)", 
    frame->label->text, frame->alignment[0], frame->alignment[1]);
}
 

void __class_method_imageframe_free(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
(void)this;
    Frame *frame = (Frame *) this->object;
    ImageFrame *imageFrame = (ImageFrame *) frame->imageFrame;
    free(imageFrame);
    frame->flags &= ~FRAME_CONTENT; // Ensure the glTexture is not deleted
    if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
    SUPER(free);
}
    

