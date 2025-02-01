#line 1 "src/__processed__/classes_nodes_frames_image_frame.class.c"
#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../storage/node.h"
#include "../io/shader.h"
#include "../render/render.h"
#include "../window.h"
#include "../gui/frame.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_IMAGEFRAME;
#line 13 "src/classes/nodes/frames/image_frame.class.c"
void __class_method_imageframe_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        call_method_0(METHOD(init_frame,this));
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

    

#line 37 "src/classes/nodes/frames/image_frame.class.c"
void __class_method_imageframe_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        this->type = __type__;
        call_method_0(METHOD(constructor,this));
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


#line 55 "src/classes/nodes/frames/image_frame.class.c"
void __class_method_imageframe_set_image(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);char * path = va_arg(args, char *);(void)this;
        Frame *frame = (Frame *) this->object;
        strcpy(frame->imageFrame->path, path);
        frame->contentTexture = load_texture_from_path(path, GL_SRGB_ALPHA, false);
        frame->flags |= FRAME_CONTENT;
}


#line 63 "src/classes/nodes/frames/image_frame.class.c"
void __class_method_imageframe_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );WorldShaders * shaders = va_arg(args, WorldShaders *);(void)this;
        SUPER(render, modelMatrix, activeShader, shaders);
}

#line 67 "src/classes/nodes/frames/image_frame.class.c"
void __class_method_imageframe_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
        fprintf(file, "(%s,%c%c)", 
        frame->label->text, frame->alignment[0], frame->alignment[1]);
}
 
#line 75 "src/classes/nodes/frames/image_frame.class.c"
void __class_method_imageframe_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        ImageFrame *imageFrame = (ImageFrame *) frame->imageFrame;
        free(imageFrame);
        frame->flags &= ~FRAME_CONTENT; // Ensure the glTexture is not deleted
        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        SUPER(free);
}
    
