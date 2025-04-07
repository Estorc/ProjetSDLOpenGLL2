/**
 * @file checkbox.class.c
 * @brief Implementation of the Checkbox class for the GUI framework.
 *
 * This source file contains the implementation of the Checkbox class, which is a part of the GUI framework
 * used in the project. The Checkbox class provides functionality to create and manage checkbox elements
 * within the graphical user interface. It handles rendering, state management, and user interaction
 * for checkboxes.
 *
 * This file is used for creating interactive checkbox components that can be used in various parts of the
 * application to allow users to make binary choices.
 *
 * @author Loup Picault
 * @date 2023-10-25
 */

#include <raptiquax.h>
#include <classes/classes.h>
#include <math/math_util.h>
#include <io/model.h>
#include <storage/node.h>
#include <io/shader.h>
#include <render/render.h>
#include <window.h>
#include <gui/frame.h>
#include <io/input.h>

/**
 * @ingroup Classes Classes
 * @{
 */
class CheckBox : public Button {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the Checkbox class.
     *
     * This function initializes a new instance of the Checkbox class. It sets up the
     * necessary properties and state for the checkbox to function correctly.
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
        frame->scale[0] = 48.0f;
        frame->scale[1] = 48.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';
    }

    /**
     * @brief Loads the resources and initializes the Checkbox.
     *
     * This function is responsible for loading the necessary resources and performing
     * any initialization required for the Checkbox. It should be called before using
     * any other functions related to the Checkbox.
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
     * @brief Checks if the current instance is a checkbox.
     *
     * This function sets the result to true if the current instance is a checkbox,
     * otherwise it sets the result to false.
     *
     * @param result A pointer to a boolean variable where the result will be stored.
     */
    void is_checkbox(bool *result) {
        *result = true;
    }

    /**
     * @brief Frees the resources used by the checkbox.
     *
     * This function is responsible for freeing the resources used by the checkbox,
     * such as textures, sounds, or any other assets loaded during the checkbox's
     * lifetime.
     */
    void free() {
        SUPER(free);
    }
 
    
}

