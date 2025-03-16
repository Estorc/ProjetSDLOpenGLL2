/**
 * @file
 * @brief This file defines the Label class and its associated functions.
 *
 * This file contains the implementation of the Label class, which is used to create and manage text labels
 * within the application. It includes functions for initializing, rendering, and updating label properties.
 * 
 * The Label class is part of the nodes/frames module and is essential for displaying text information
 * in the graphical user interface.
 *
 * Additional details:
 * - The Label class supports various text styles and alignments.
 * - It integrates with the SDL and OpenGL libraries for rendering.
 * - The class provides methods to handle dynamic text updates.
 *
 * @author Loup Picault
 * @date 2023-10-10
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

/**
 * @ingroup Classes Classes
 * @{
 */
class Label : public Frame {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the label class.
     *
     * This function initializes a label object with the specified text and alignment parameters.
     *
     * @param text The text to be displayed in the label.
     * @param ha The horizontal alignment of the text. 
     *           Possible values:
     *           - 0: Left-aligned
     *           - 1: Center-aligned
     *           - 2: Right-aligned
     * @param va The vertical alignment of the text.
     *           Possible values:
     *           - 0: Top-aligned
     *           - 1: Middle-aligned
     *           - 2: Bottom-aligned
     */
    void constructor(char *text, int ha, int va) {
        this->type = __type__; 

        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();
        frame->label = malloc(sizeof(Label));
        POINTER_CHECK(frame->label);
        frame->label->displayLength = -1;
        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->scale[0] = 100.0f;
        frame->scale[1] = 100.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';
        frame->unit[2] = '%';
        frame->unit[3] = '%';
        frame->label->text = NULL;
        frame->flags |= FRAME_CONTENT;
        glGenTextures(1, &frame->contentTexture);

        frame->label->text = malloc(strlen(text) + 1);
        POINTER_CHECK(frame->label->text);
        strcpy(frame->label->text, text);

        frame->alignment[0] = ha;
        frame->alignment[1] = va;
    }

    

    void load(FILE *file) {
        static char text[2048]; // In static to prevent stack overflow
        char alignment[2];
        if (file) {
            fscanf(file, "(\"%[^\"]\",%c%c)", 
            text, &alignment[0], &alignment[1]);
            format_escaped_newlines(text);
        }
        this::constructor(text, alignment[0], alignment[1]);
    }

    void set_text(char * str) {
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;
        label->text = realloc(label->text, strlen(str) + 1);
        strcpy(label->text, str);
        frame->flags |= FRAME_NEEDS_REFRESH;
    }


    void refresh() {
        SUPER(refresh);
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;

        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        frame->contentSurface = SDL_CreateRGBSurface(0,frame->size[0],frame->size[1],32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (!frame->contentSurface) {
            PRINT_ERROR("Error creating surface: %s\n", SDL_GetError());
        }
        if (label->text[0]) {
            char deletedChar;
            if (label->displayLength >= 0) {
                deletedChar = label->text[label->displayLength];
                label->text[label->displayLength] = '\0';
            }
            draw_text(frame->contentSurface, 0, 0, label->text, frame->theme->font.font, frame->theme->textColor, frame->alignment, (int) frame->size[0]);
            if (label->displayLength >= 0) {
                label->text[label->displayLength] = deletedChar;
            }
        }
        SUPER(refreshContent);
    }


    void render(mat4 *modelMatrix, Shader activeShader, WorldShaders *shaders) {
        SUPER(render, modelMatrix, activeShader, shaders);
    }

    void save(FILE *file, Node *editor) {
        Frame *frame = (Frame *) this->object;
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
        fprintf(file, "(%s,%c%c)", 
        frame->label->text, frame->alignment[0], frame->alignment[1]);
    }
 
    void free() {
        Frame *frame = (Frame *) this->object;
        Label *label = (Label *) frame->label;
        free(label->text);
        free(label);
        SUPER(free);
    }
    
}

