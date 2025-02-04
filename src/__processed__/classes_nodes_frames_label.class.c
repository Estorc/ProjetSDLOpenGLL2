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
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_LABEL;
#line 13 "src/classes/nodes/frames/label.class.c"
void __class_method_label_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
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

    

#line 38 "src/classes/nodes/frames/label.class.c"
void __class_method_label_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        this->type = __type__;
        this::constructor();
        Frame *frame = (Frame *) this->object;
        if (file) {
            fscanf(file, "(%[^,],%c%c)", 
            frame->label->text, &frame->alignment[0], &frame->alignment[1]);
        }
}

#line 48 "src/classes/nodes/frames/label.class.c"
void __class_method_label_set_text(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);char *  str = va_arg(args, char * );(void)this;
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;
        strcpy(label->text, str);
        frame->flags |= FRAME_NEEDS_REFRESH;
}


#line 56 "src/classes/nodes/frames/label.class.c"
void __class_method_label_refresh(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        SUPER(refresh);
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;

        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        frame->contentSurface = SDL_CreateRGBSurface(0,frame->size[0],frame->size[1],32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (!frame->contentSurface) {
            PRINT_ERROR("Error creating surface: %s\n", SDL_GetError());
        }
        if (label->text[0]) draw_text(frame->contentSurface, 0, 0, label->text, frame->theme->font.font, frame->theme->textColor, frame->alignment, -1);
        SUPER(refreshContent);
}


#line 71 "src/classes/nodes/frames/label.class.c"
void __class_method_label_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );WorldShaders * shaders = va_arg(args, WorldShaders *);(void)this;
        SUPER(render, modelMatrix, activeShader, shaders);
}

#line 75 "src/classes/nodes/frames/label.class.c"
void __class_method_label_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
        fprintf(file, "(%s,%c%c)", 
        frame->label->text, frame->alignment[0], frame->alignment[1]);
}
 
#line 83 "src/classes/nodes/frames/label.class.c"
void __class_method_label_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;
        free(label);
        SUPER(free);
}
    
