#line 1 "src/__processed__/classes_nodes_frames_selectlist.class.c"
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
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_SELECTLIST;
#line 14 "src/classes/nodes/frames/selectlist.class.c"
void __class_method_selectlist_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        call_method_0(METHOD(init_frame,this));

        frame->selectList = malloc(sizeof(SelectList));
        POINTER_CHECK(frame->selectList);
        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->scale[0] = 100.0f;
        frame->scale[1] = 100.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';
        frame->unit[2] = '%';
        frame->unit[3] = '%';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';
        frame->selectList->state = BUTTON_STATE_NORMAL;
        frame->selectList->options = (char (*)[256]) "Default Option";
        frame->selectList->count = 1;
        frame->selectList->selected = NULL;
        frame->flags |= FRAME_CONTENT;
        frame->flags |= FRAME_BACKGROUND;
        glGenTextures(1, &frame->contentTexture);
}

    

#line 47 "src/classes/nodes/frames/selectlist.class.c"
void __class_method_selectlist_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        IGNORE(file);
        this->type = __type__;
        call_method_0(METHOD(constructor,this));
}

#line 53 "src/classes/nodes/frames/selectlist.class.c"
void __class_method_selectlist_refreshOptions(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        for (int i = 0; i < this->length; i++) {
            call_method_0(METHOD(free,(this->children[i])));
        }
        call_method_0(METHOD(refresh,this));
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = (SelectList *) frame->selectList;
        this->children = realloc(this->children, sizeof(Node *));
        frame->flags |= OVERFLOW_VISIBLE;


        Node *listFrame = malloc(sizeof(Node));
        POINTER_CHECK(listFrame);
        call_method_0(METHOD_TYPE(CLASS_TYPE_FRAME,constructor,listFrame));
        listFrame->flags &= ~NODE_VISIBLE;
        listFrame->flags &= ~NODE_ACTIVE;
        Frame *listFrameFrame = (Frame *) listFrame->object;
        listFrameFrame->relPos[0] = 0.0f;
        listFrameFrame->relPos[1] = 100.0f;
        listFrameFrame->scale[0] = 100.0f;
        listFrameFrame->scale[1] = 600.0f;
        listFrameFrame->unit[0] = '%';
        listFrameFrame->unit[1] = '%';
        listFrameFrame->unit[2] = '%';
        listFrameFrame->unit[3] = '%';
        listFrameFrame->alignment[0] = 'l';
        listFrameFrame->alignment[1] = 't';
        listFrameFrame->theme = frame->theme;
        listFrameFrame->flags |= FRAME_BACKGROUND;
        listFrame->children = realloc(listFrame->children, sizeof(Node *));

        Node *list = malloc(sizeof(Node));
        POINTER_CHECK(list);
        call_method_0(METHOD_TYPE(CLASS_TYPE_CONTROLFRAME,constructor,list));
        Frame *listFrameControl = (Frame *) list->object;
        listFrameControl->relPos[0] = 2.0f;
        listFrameControl->relPos[1] = 2.0f;
        listFrameControl->scale[0] = 96.0f;
        listFrameControl->scale[1] = 96.0f;
        listFrameControl->unit[0] = '%';
        listFrameControl->unit[1] = '%';
        listFrameControl->unit[2] = '%';
        listFrameControl->unit[3] = '%';
        listFrameControl->alignment[0] = 'l';
        listFrameControl->alignment[1] = 't';
        listFrameControl->theme = frame->theme;
        listFrameControl->flags |= FRAME_BACKGROUND;
        listFrameControl->flags |= OVERFLOW_SCROLL;
        list->children = realloc(list->children, sizeof(Node *) * selectList->count);

        call_method_0(METHOD(add_child,listFrame,list));
        call_method_0(METHOD(add_child,this,listFrame));

        for (int i = 0; i < selectList->count; i++) {

            Node *child = malloc(sizeof(Node));
            POINTER_CHECK(child);
            call_method_0(METHOD_TYPE(CLASS_TYPE_CONTROLFRAME,constructor,child));
            Frame *childFrame = (Frame *) child->object;
            childFrame->relPos[0] = 0.0f;
            childFrame->relPos[1] = frame->size[1]*i;
            childFrame->scale[0] = 100.0f;
            childFrame->scale[1] = frame->size[1];
            childFrame->unit[0] = '%';
            childFrame->unit[1] = 'p';
            childFrame->unit[2] = '%';
            childFrame->unit[3] = 'p';
            childFrame->alignment[0] = 'l';
            childFrame->alignment[1] = 't';
            childFrame->theme = frame->theme;
            child->children = realloc(child->children, sizeof(Node *));
            call_method_0(METHOD(add_child,list,child));

            Node *button = malloc(sizeof(Node));
            POINTER_CHECK(button);
            call_method_0(METHOD_TYPE(CLASS_TYPE_BUTTON,constructor,button));
            call_method_0(METHOD(init_button,button));
            button->children = realloc(button->children, sizeof(Node *));

            Node *label = malloc(sizeof(Node));
            POINTER_CHECK(label);
            call_method_0(METHOD_TYPE(CLASS_TYPE_LABEL,constructor,label));
            Frame *labelFrame = (Frame *) label->object;
            strcpy(labelFrame->label->text, selectList->options[i]);
            labelFrame->relPos[0] = 5.0f;
            labelFrame->alignment[0] = 'l';
            labelFrame->alignment[1] = 'c';

            call_method_0(METHOD(add_child,child,button));
            call_method_0(METHOD(add_child,button,label));
        }
        call_method_0(METHOD(print,this,0));
}


#line 148 "src/classes/nodes/frames/selectlist.class.c"
void __class_method_selectlist_refresh(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        SUPER(refresh);
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = (SelectList *) frame->selectList;

        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        frame->contentSurface = SDL_CreateRGBSurface(0,frame->size[0],frame->size[1],32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (!frame->contentSurface) {
            printf("Error creating surface: %s\n", SDL_GetError());
        }
        if (selectList->selected && selectList->options[*selectList->selected][0]) draw_text(frame->contentSurface, 0, 0, selectList->options[*selectList->selected], frame->theme->font.font, frame->theme->textColor, frame->alignment, -1);
        else if (selectList->options[0][0]) draw_text(frame->contentSurface, 0, 0, selectList->options[0], frame->theme->font.font, (SDL_Color) {frame->theme->textColor.r, frame->theme->textColor.g, frame->theme->textColor.b, frame->theme->textColor.a*0.8}, frame->alignment, -1);
        SUPER(refreshContent);
}


#line 164 "src/classes/nodes/frames/selectlist.class.c"
void __class_method_selectlist_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = (SelectList *) frame->selectList;
        float x,y,w,h;
        x = frame->absPos[0];
        y = frame->absPos[1];
        w = frame->size[0];
        h = frame->size[1];
        if (selectList->state == BUTTON_STATE_PRESSED) {
            if (this->children) {
                this->children[0]->flags |= NODE_VISIBLE;
                this->children[0]->flags |= NODE_ACTIVE;
            }
            if (input.released_keys & KEY_ENTER || ((input.mouse.released_button == SDL_BUTTON_LEFT || input.mouse.pressed_button == SDL_BUTTON_LEFT) && !(input.mouse.x > x &&
                input.mouse.x < x+w &&
                input.mouse.y > y &&
                input.mouse.y < y+h))) {
                    for (int i = 0; i < this->children[0]->children[0]->length; i++) {
                        Frame *childFrame = (Frame *) this->children[0]->children[0]->children[i]->children[0]->object;
                        Button *button = (Button *) childFrame->button;
                        if (button->state != BUTTON_STATE_NORMAL) {
                            if (selectList->selected) *selectList->selected = i;
                            break;
                        }
                    }
                    selectList->state = BUTTON_STATE_NORMAL;
                    call_method_0(METHOD(refresh,this));
                }
        } else {
            if (this->children) {
                this->children[0]->flags &= ~NODE_VISIBLE;
                this->children[0]->flags &= ~NODE_ACTIVE;
            }
            if (input.mouse.x > x &&
                input.mouse.x < x+w &&
                input.mouse.y > y &&
                input.mouse.y < y+h) {
                
                selectList->state = BUTTON_STATE_HOVERED;

                if (input.mouse.pressed_button == SDL_BUTTON_LEFT) {
                    selectList->state = BUTTON_STATE_PRESSED;
                }

            } else {
                selectList->state = BUTTON_STATE_NORMAL;
            }
        }
        if (window.resized) frame->flags |= FRAME_NEEDS_REFRESH;
}

#line 215 "src/classes/nodes/frames/selectlist.class.c"
void __class_method_selectlist_is_selectlist(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = true;
}

#line 219 "src/classes/nodes/frames/selectlist.class.c"
void __class_method_selectlist_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
}

#line 224 "src/classes/nodes/frames/selectlist.class.c"
void __class_method_selectlist_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = frame->selectList;
        free(selectList);
        SUPER(free);
}
 
    
