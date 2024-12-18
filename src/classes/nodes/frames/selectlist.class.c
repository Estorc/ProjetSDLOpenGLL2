#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window.h"
#include "gui/frame.h"
#include "io/input.h"

class SelectList @promote extends Frame {
    __containerType__ Node *

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
        IGNORE(file);
        METHOD_TYPE(this, __type__, constructor);
        Frame *frame = (Frame *) this->object;
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
    }

    void refreshOptions() {
        for (int i = 0; i < this->length; i++) {
            METHOD(this->children[i], free);
        }
        METHOD(this, refresh);
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = (SelectList *) frame->selectList;
        this->children = realloc(this->children, sizeof(Node *));
        frame->flags |= OVERFLOW_VISIBLE;


        Node *listFrame = malloc(sizeof(Node));
        POINTER_CHECK(listFrame);
        METHOD_TYPE(listFrame, CLASS_TYPE_FRAME, constructor);
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
        METHOD_TYPE(list, CLASS_TYPE_CONTROLFRAME, constructor);
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

        add_child(listFrame, list);
        add_child(this, listFrame);

        for (int i = 0; i < selectList->count; i++) {

            Node *child = malloc(sizeof(Node));
            POINTER_CHECK(child);
            METHOD_TYPE(child, CLASS_TYPE_CONTROLFRAME, constructor);
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
            add_child(list, child);

            Node *button = malloc(sizeof(Node));
            POINTER_CHECK(button);
            METHOD_TYPE(button, CLASS_TYPE_BUTTON, constructor);
            METHOD(button, init_button);
            button->children = realloc(button->children, sizeof(Node *));

            Node *label = malloc(sizeof(Node));
            POINTER_CHECK(label);
            METHOD_TYPE(label, CLASS_TYPE_LABEL, constructor);
            Frame *labelFrame = (Frame *) label->object;
            strcpy(labelFrame->label->text, selectList->options[i]);
            labelFrame->relPos[0] = 5.0f;
            labelFrame->alignment[0] = 'l';
            labelFrame->alignment[1] = 'c';

            add_child(child, button);
            add_child(button, label);
        }
        print_node(this, 0);
    }


    void refresh() {
        SUPER(refresh);
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = (SelectList *) frame->selectList;

        if (selectList->selected && selectList->options[*selectList->selected][0]) draw_text(frame->contentSurface, 0, 0, selectList->options[*selectList->selected], frame->theme->font.font, frame->theme->textColor, frame->alignment, -1);
        else if (selectList->options[0][0]) draw_text(frame->contentSurface, 0, 0, selectList->options[0], frame->theme->font.font, (SDL_Color) {frame->theme->textColor.r, frame->theme->textColor.g, frame->theme->textColor.b, frame->theme->textColor.a*0.8}, frame->alignment, -1);
        SUPER(refreshContent);
    }


    void update() {
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
                            printf("Selected: %d\n", i);
                            printf("True Selected: %d\n", *selectList->selected);
                            break;
                        }
                    }
                    selectList->state = BUTTON_STATE_NORMAL;
                    METHOD(this, refresh);
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

    void is_selectlist(bool *result) {
        *result = true;
    }

    void save(FILE *file, Node *editor) {
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    void free() {
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = frame->selectList;
        free(selectList);
        SUPER(free);
    }
 
    
}