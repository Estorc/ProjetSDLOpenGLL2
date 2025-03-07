/**
 * @file
 * @brief This file defines the InputArea class and its associated methods.
 *
 * This source file contains the implementation of the InputArea class, which is used
 * to handle user input within a specific area of the application. It provides methods
 * for initializing, updating, and rendering the input area, as well as handling
 * user interactions such as keyboard and mouse events.
 *
 * The InputArea class is designed to be a part of a larger application that uses SDL
 * and OpenGL for rendering and user interface management.
 *
 * @author Loup Picault
 * @date 2023-10-20
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include "window.h"
#include "gui/frame.h"
#include "io/input.h"

class InputArea : public Frame {
    __containerType__ Node *
    public:

    void constructor(char *defaultText, int ha, int va) {
        this->type = __type__; 

        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();
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
        strcpy(frame->inputArea->defaultText, defaultText);
        frame->inputArea->text[0] = 0;
        frame->alignment[0] = ha;
        frame->alignment[1] = va;
    }

    

    void load(FILE *file) {
        char defaultText[100];
        char alignment[2];
        if (file) {
            fscanf(file, "(%[^,],%c%c)", 
            defaultText, &alignment[0], &alignment[1]);
        }
        this::constructor(defaultText, alignment[0], alignment[1]);
    }

    void refresh() {
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

    void update() {
        Frame *frame = (Frame *) this->object;
        InputArea *inputArea = (InputArea *) frame->inputArea;
        float x,y,w,h;
        x = frame->absPos[0];
        y = frame->absPos[1];
        w = frame->size[0];
        h = frame->size[1];
        if (inputArea->state == BUTTON_STATE_PRESSED) {
            if (Game.input->text_input) {
                sprintf(inputArea->text, "%s", Game.input->inputBuffer);
                this::refresh();
            }
            if (Game.input->released_keys & KEY_ENTER || ((Game.input->mouse.released_button == SDL_BUTTON_LEFT || Game.input->mouse.pressed_button == SDL_BUTTON_LEFT) && !(Game.input->mouse.x > x &&
                Game.input->mouse.x < x+w &&
                Game.input->mouse.y > y &&
                Game.input->mouse.y < y+h)))
                inputArea->state = BUTTON_STATE_NORMAL;
        } else {
            if (Game.input->mouse.x > x &&
                Game.input->mouse.x < x+w &&
                Game.input->mouse.y > y &&
                Game.input->mouse.y < y+h) {

                inputArea->state = BUTTON_STATE_HOVERED;

                if (Game.input->mouse.pressed_button == SDL_BUTTON_LEFT) {
                    inputArea->state = BUTTON_STATE_PRESSED;
                    SDL_StartTextInput();
                    strcpy(Game.input->inputBuffer, inputArea->text);
                }

            } else {
                inputArea->state = BUTTON_STATE_NORMAL;
            }
        }
        if (Game.window->resized) frame->flags |= FRAME_NEEDS_REFRESH;
    }

    void is_input_area(bool *result) {
        *result = true;
    }

    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    void free() {
        Frame *frame = (Frame *) this->object;
        InputArea *inputArea = frame->inputArea;
        free(inputArea);
        SUPER(free);
    }
 
    
}