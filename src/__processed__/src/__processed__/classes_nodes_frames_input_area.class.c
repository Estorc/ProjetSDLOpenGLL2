#line 1 "src/__processed__/classes_nodes_frames_input_area.class.c"
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
#include "../io/input.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_INPUTAREA;
#line 14 "src/classes/nodes/frames/input_area.class.c"
void __class_method_inputarea_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        call_method_0(METHOD(init_frame,this));
        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->scale[0] = 100.0f;
        frame->scale[1] = 100.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';
        frame->unit[2] = '%';
        frame->unit[3] = '%';
        frame->flags |= FRAME_CONTENT;
        frame->inputArea = malloc(sizeof(InputArea));
        POINTER_CHECK(frame->inputArea);
        glGenTextures(1, &frame->contentTexture);
}

    

#line 39 "src/classes/nodes/frames/input_area.class.c"
void __class_method_inputarea_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        this->type = __type__;
        call_method_0(METHOD(constructor,this));
        Frame *frame = (Frame *) this->object;
        if (file) {
            fscanf(file, "(%[^,],%c%c)", 
            frame->inputArea->defaultText, &frame->alignment[0], &frame->alignment[1]);
            frame->inputArea->text[0] = '\0';
        }
}

#line 50 "src/classes/nodes/frames/input_area.class.c"
void __class_method_inputarea_refresh(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        SUPER(refresh);
        Frame *frame = (Frame *) this->object;
        InputArea *inputArea = (InputArea *) frame->inputArea;

        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        frame->contentSurface = SDL_CreateRGBSurface(0,frame->size[0],frame->size[1],32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (!frame->contentSurface) {
            PRINT_ERROR("Error creating surface: %s\n", SDL_GetError());
        }
        if (inputArea->text[0]) draw_text(frame->contentSurface, 0, 0, inputArea->text, frame->theme->font.font, frame->theme->textColor, frame->alignment, -1);
        else draw_text(frame->contentSurface, 0, 0, inputArea->defaultText, frame->theme->font.font, (SDL_Color) {frame->theme->textColor.r, frame->theme->textColor.g, frame->theme->textColor.b, frame->theme->textColor.a*0.8}, frame->alignment, -1);
        SUPER(refreshContent);
}

#line 65 "src/classes/nodes/frames/input_area.class.c"
void __class_method_inputarea_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        InputArea *inputArea = (InputArea *) frame->inputArea;
        float x,y,w,h;
        x = frame->absPos[0];
        y = frame->absPos[1];
        w = frame->size[0];
        h = frame->size[1];
        if (inputArea->state == BUTTON_STATE_PRESSED) {
            if (input.text_input) {
                sprintf(inputArea->text, "%s", input.inputBuffer);
                call_method_0(METHOD(refresh,this));
            }
            if (input.released_keys & KEY_ENTER || ((input.mouse.released_button == SDL_BUTTON_LEFT || input.mouse.pressed_button == SDL_BUTTON_LEFT) && !(input.mouse.x > x &&
                input.mouse.x < x+w &&
                input.mouse.y > y &&
                input.mouse.y < y+h)))
                inputArea->state = BUTTON_STATE_NORMAL;
        } else {
            if (input.mouse.x > x &&
                input.mouse.x < x+w &&
                input.mouse.y > y &&
                input.mouse.y < y+h) {

                inputArea->state = BUTTON_STATE_HOVERED;

                if (input.mouse.pressed_button == SDL_BUTTON_LEFT) {
                    inputArea->state = BUTTON_STATE_PRESSED;
                    SDL_StartTextInput();
                    strcpy(input.inputBuffer, inputArea->text);
                }

            } else {
                inputArea->state = BUTTON_STATE_NORMAL;
            }
        }
        if (window.resized) frame->flags |= FRAME_NEEDS_REFRESH;
}

#line 104 "src/classes/nodes/frames/input_area.class.c"
void __class_method_inputarea_is_input_area(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = true;
}

#line 108 "src/classes/nodes/frames/input_area.class.c"
void __class_method_inputarea_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
}

#line 113 "src/classes/nodes/frames/input_area.class.c"
void __class_method_inputarea_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        InputArea *inputArea = frame->inputArea;
        free(inputArea);
        SUPER(free);
}
 
    
