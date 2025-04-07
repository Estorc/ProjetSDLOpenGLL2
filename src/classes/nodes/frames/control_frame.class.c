/**
 * @file
 * @brief This file defines the implementation of the control frame class.
 *
 * This source file contains the implementation details for the control frame class,
 * which is responsible for managing and rendering control frames within the application.
 * It includes functions for initializing, updating, and rendering control frames,
 * as well as handling user input and interactions.
 *
 * @author Loup Picault
 * @date October 6, 2023
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

/**
 * @ingroup Classes Classes
 * @{
 */
class ControlFrame : public Frame {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the control frame class.
     *
     * This function initializes a control frame with the specified parameters.
     *
     * @param x The x-coordinate of the control frame.
     * @param xu The unit of measurement for the x-coordinate.
     * @param y The y-coordinate of the control frame.
     * @param yu The unit of measurement for the y-coordinate.
     * @param w The width of the control frame.
     * @param wu The unit of measurement for the width.
     * @param h The height of the control frame.
     * @param hu The unit of measurement for the height.
     * @param ha The horizontal alignment of the control frame.
     * @param va The vertical alignment of the control frame.
     * @param scroll The scroll setting for the control frame.
     */
    void constructor(float x, int xu, float y, int yu, float w, int wu, float h, int hu, int ha, int va, int scroll, Theme *theme) {
        this->type = __type__; 

        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();

        frame->relPos[0] = x;
        frame->relPos[1] = y;
        frame->scale[0] = w;
        frame->scale[1] = h;
        frame->unit[0] = xu;
        frame->unit[1] = yu;
        frame->unit[2] = wu;
        frame->unit[3] = hu;
        frame->alignment[0] = ha;
        frame->alignment[1] = va;

        if (scroll == 's') {
            frame->flags |= OVERFLOW_SCROLL;
        } else if (scroll == 'v') {
            frame->flags |= OVERFLOW_VISIBLE;
        }

        frame->theme = theme;
    }

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the given file pointer and processes it accordingly.
     *
     * @param file A pointer to the FILE object that represents the file to be read.
     */
    void load(FILE *file) {
        Frame::load(this, file);
    }

    /**
     * @brief Renders the control frame.
     *
     * This function is responsible for rendering the control frame. It performs all necessary
     * operations to display the control frame on the screen.
     */
    void render() {
        Frame *frame = (Frame *) this->object;
        if (frame->flags & FRAME_NEEDS_REFRESH) this::refresh();
    }

    /**
     * @brief Saves the state of the editor node to a file.
     *
     * This function writes the current state of the provided editor node to the specified file.
     * It is used to persist the state of the editor for later retrieval.
     *
     * @param file A pointer to the FILE object where the editor state will be saved.
     * @param editor A pointer to the Node object representing the editor whose state is to be saved.
     */
    void save(FILE *file, Node *editor) {
        Frame *frame = (Frame *) this->object;
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);

        fprintf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c%c)", 
            frame->relPos[0],frame->unit[0], 
            frame->relPos[1],frame->unit[1], 
            frame->scale[0],frame->unit[2], 
            frame->scale[1],frame->unit[3], 
            frame->alignment[0], frame->alignment[1],
            (frame->flags & OVERFLOW_SCROLL) ? 's' : (frame->flags & OVERFLOW_VISIBLE) ? 'v' : 'n');
    }
    
}

