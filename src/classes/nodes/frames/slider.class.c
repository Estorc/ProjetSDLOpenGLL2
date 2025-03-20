/**
 * @file radiobutton.class.c
 * @brief Implementation of the RadioButton class.
 *
 * This source file contains the implementation of the RadioButton class, which is used to create and manage radio button UI elements in the application. The RadioButton class provides functionality to handle user interactions, rendering, and state management of radio buttons.
 *
 * This file is part of the larger project that integrates SDL and OpenGL for rendering graphical user interfaces.
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

/**
 * @ingroup Classes Classes
 * @{
 */
class Slider : public Button {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the slider class.
     *
     * This function initializes a slider object with the specified minimum and maximum values.
     *
     * @param min The minimum value of the slider.
     * @param max The maximum value of the slider.
     */
    void constructor(float min, float max) {
        this->type = __type__; 
        
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);
        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();
        this::init_slider();

        frame->relPos[0] = 0.0f;
        frame->relPos[1] = 0.0f;
        frame->unit[0] = '!';
        frame->unit[1] = '%';

        frame->scale[0] = 24.0f;
        frame->scale[1] = 24.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';

        frame->slider->min = min;
        frame->slider->max = max;
    }

    /**
     * @brief Initializes the slider.
     *
     * This function sets up the initial state of the slider, including any necessary
     * resources or configurations. It should be called before using the slider.
     */
    void init_slider() {
        Frame *frame = (Frame *) this->object;
        frame->slider = malloc(sizeof(Slider));
        Slider *slider = frame->slider;
        slider->checked = NULL;
        slider->value = NULL;
        slider->min = 0.0f;
        slider->max = 1.0f;
    }

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the given file and initializes the slider
     * class with the loaded data.
     *
     * @param file A pointer to the file to read from.
     */
    void load(FILE *file) {
        float min, max;
        if (file) {
            fscanf(file, "(%g,%g)", 
            &min, &max);
        }
        this::constructor(min, max);
    }

    /**
     * @brief Saves the state of the editor node to a file.
     *
     * This function writes the current state of the editor node to the specified file.
     *
     * @param file A pointer to the FILE object where the state will be saved.
     * @param editor A pointer to the Node object representing the editor whose state is to be saved.
     */
    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        Frame *frame = (Frame *) this->object;
        fprintf(file, "%s(%g,%g)", classManager.class_names[this->type],
        frame->slider->min, frame->slider->max);
    }

    /**
     * @brief Checks if the current node is a radio button.
     *
     * This function determines whether the current node in the GUI framework
     * is a radio button. It returns a boolean value indicating the result.
     *
     * @return true if the current node is a radio button, false otherwise.
     */
    bool is_radiobutton() {
        return true;
    }

    /**
     * @brief Updates the state of the slider.
     *
     * This function is responsible for updating the state of the slider based on
     * user interactions or other events. It should be called regularly to ensure
     * the slider's state is kept up to date.
     */
    void update() {
        Frame * frame = this->object;
        Frame * parentFrame = this->parent->object;

        SUPER(update);
        if (frame->slider->state == BUTTON_STATE_PRESSED) {
            float w = parentFrame->size[0];
            float value = (Game.input->mouse.x - parentFrame->absPos[0]) / w;
            *frame->slider->value = frame->slider->min + (frame->slider->max - frame->slider->min) * value;
            *frame->slider->value = CLAMP(frame->slider->min, *frame->slider->value, frame->slider->max);
        }
        float percent = (*frame->slider->value - frame->slider->min) / (frame->slider->max - frame->slider->min);
        if (frame->slider->state != BUTTON_STATE_PRESSED) {
            frame->absPos[0] = parentFrame->absPos[0] + parentFrame->size[0] * percent - frame->size[0]/2;
        }
        this->pos[0] = -this->parent->globalScale[0] + this->parent->globalScale[0] * 2.0f * percent;
    }

    /**
     * @brief Frees the resources allocated by the slider class.
     *
     * This function is responsible for releasing any memory or resources
     * that were allocated for the slider class. It should be called when
     * the slider is no longer needed to prevent memory leaks.
     */
    void free() {
        SUPER(free);
    }
 
    
}

