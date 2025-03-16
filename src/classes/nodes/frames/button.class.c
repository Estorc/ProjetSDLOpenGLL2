/**
 * @file button.class.c
 * @brief This file defines the Button class and its associated methods.
 *
 * This source file contains the implementation of the Button class, which is used
 * to create and manage button elements within the application. The Button class
 * provides functionalities such as rendering the button, handling user interactions,
 * and triggering events when the button is clicked.
 *
 * The Button class is a crucial component for creating interactive user interfaces
 * in the application, allowing users to perform actions through button clicks.
 *
 * @author Loup Picault
 * @date 2023-10-31
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

/**
 * @ingroup Classes Classes
 * @{
 */
class Button : public Frame {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the Button class.
     *
     * This function initializes a new instance of the Button class.
     */
    void constructor() {
        this->type = __type__; 

        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();
        this::init_button();
    }

    /**
     * @brief Initializes the button.
     *
     * This function sets up the initial state and properties of the button.
     * It should be called before using the button in the application.
     */
    void init_button() {
        Frame *frame = (Frame *) this->object;
        frame->button = malloc(sizeof(Button));
        POINTER_CHECK(frame->button);
        frame->button->checked = NULL;
        frame->button->state = BUTTON_STATE_NORMAL;
        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->scale[0] = 100.0f;
        frame->scale[1] = 100.0f;
        frame->unit[0] = '%';
        frame->unit[1] = '%';
        frame->unit[2] = '%';
        frame->unit[3] = '%';
    }
    
    /**
     * @brief Loads the button resources.
     *
     * This function is responsible for loading the necessary resources for the button,
     * such as textures, sounds, or any other assets required for the button to function
     * properly within the application.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Updates the state of the button.
     *
     * This function is responsible for updating the button's state, which may
     * include handling user input, updating the button's appearance, and
     * triggering any associated actions or events.
     */
    void update() {
        Frame *frame = (Frame *) this->object;
        Button *button = (Button *) frame->button;
        Mouse *mouse = &Game.input->mouse;
        if (Game.renderTarget) {
            mouse = &Game.renderTarget->mouse;
        }
        float x,y,w,h;
        x = frame->absPos[0];
        y = frame->absPos[1];
        w = frame->size[0];
        h = frame->size[1];
        if (button->state == BUTTON_STATE_PRESSED) {
            if (mouse->active_button != SDL_BUTTON_LEFT) {
                button->state = BUTTON_STATE_NORMAL;
                if (mouse->x > x &&
                    mouse->x < x+w &&
                    mouse->y > y &&
                    mouse->y < y+h &&
                    mouse->x > frame->overflow[0] &&
                    mouse->x < frame->overflow[2] &&
                    mouse->y > frame->overflow[1] &&
                    mouse->y < frame->overflow[3]) {
                    if (button->checked) *button->checked = !(*button->checked);
                    this::emit_signal(SIGNAL_BUTTON_CLICKED);
                }
            }
        } else {
            if (mouse->x > x &&
                mouse->x < x+w &&
                mouse->y > y &&
                mouse->y < y+h &&
                mouse->x > frame->overflow[0] &&
                mouse->x < frame->overflow[2] &&
                mouse->y > frame->overflow[1] &&
                mouse->y < frame->overflow[3]) {

                if (button->state != BUTTON_STATE_HOVERED) {
                    this::emit_signal(SIGNAL_BUTTON_HOVERED);
                    button->state = BUTTON_STATE_HOVERED;
                }

                if (mouse->pressed_button == SDL_BUTTON_LEFT)
                    button->state = BUTTON_STATE_PRESSED;

            } else {
                button->state = BUTTON_STATE_NORMAL;
            }
        }
    }

    /**
     * @brief Checks if the current instance is a button.
     *
     * This function sets the result to true if the current instance is a button,
     * otherwise it sets the result to false.
     *
     * @param result A pointer to a boolean variable where the result will be stored.
     */
    void is_button(bool *result) {
        *result = true;
    }

    /**
     * @brief Saves the state of the editor node to a file.
     *
     * This function writes the current state of the editor node to the specified file.
     *
     * @param file A pointer to the FILE object where the editor state will be saved.
     * @param editor A pointer to the Node object representing the editor whose state is to be saved.
     */
    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    /**
     * @brief Frees the resources used by the button.
     *
     * This function is responsible for freeing the resources used by the button,
     * such as textures, sounds, or any other assets loaded during the button's
     * lifetime.
     */
    void free() {
        Frame *frame = (Frame *) this->object;
        free(frame->button);
        SUPER(free);
    }
 
    
}

