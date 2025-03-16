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
class RadioButton : public Button {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the RadioButton class.
     *
     * This function initializes a new instance of the RadioButton class.
     * It sets up the necessary properties and states for the radio button.
     */
    void constructor() {
        this->type = __type__; 

        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);
        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();
        this::init_radiobutton();
        frame->scale[0] = 48.0f;
        frame->scale[1] = 48.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';
    }

    /**
     * @brief Initializes the radio button.
     *
     * This function sets up the necessary properties and states for the radio button
     * to function correctly. It should be called before using any other radio button
     * functions.
     */
    void init_radiobutton() {
        Frame *frame = (Frame *) this->object;
        frame->radiobutton = malloc(sizeof(RadioButton));
        RadioButton *radiobutton = frame->radiobutton;
        radiobutton->checked = NULL;
    }

    /**
     * @brief Loads the necessary resources or initializes the RadioButton.
     *
     * This function is responsible for loading any resources or performing
     * any initialization required for the RadioButton class. The specific
     * details of what is loaded or initialized should be documented here.
     */
    void load() {
        this::constructor();
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
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    /**
     * @brief Checks if the current object is a radiobutton.
     *
     * This function determines whether the current object is of type radiobutton.
     *
     * @return true if the object is a radiobutton, false otherwise.
     */
    bool is_radiobutton() {
        return true;
    }

    /**
     * @brief Frees the resources allocated for the RadioButton instance.
     *
     * This function is responsible for releasing any memory or resources that were allocated
     * for the RadioButton instance. It should be called when the RadioButton is no longer needed
     * to avoid memory leaks.
     */
    void free() {
        SUPER(free);
    }
 
    
}

