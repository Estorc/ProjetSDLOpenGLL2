/**
 * @file
 * @brief This file implements the functionality for a selectable list frame in the application.
 *
 * This source file contains the implementation of a selectable list frame, which is used to
 * display and manage a list of selectable items within the application. It provides the necessary
 * functions to create, update, and handle user interactions with the list.
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
#include <io/input.h>

/**
 * @ingroup Classes Classes
 * @{
 */
class SelectList : public Frame {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the select list class.
     *
     * This function initializes the select list class, setting up any necessary
     * data structures and preparing the class for use.
     */
    void constructor() {
        this->type = __type__; 

        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();

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

    /**
     * @brief Loads the select list.
     *
     * This function is responsible for initializing and loading the select list.
     * It sets up the necessary data structures and prepares the list for use.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Refreshes the options in the select list.
     *
     * This function is responsible for updating the options available in the select list. It ensures that the list is current and reflects any changes that may have occurred.
     */
    void refreshOptions() {
        for (int i = 0; i < this->length; i++) {
            (this->children[i])::free();
        }
        this::refresh();
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = (SelectList *) frame->selectList;
        this->children = realloc(this->children, sizeof(Node *));
        frame->flags |= OVERFLOW_VISIBLE;


        Node *listFrame = malloc(sizeof(Node));
        POINTER_CHECK(listFrame);
        listFrame = new Frame(0.0f, '%', 100.0f, '%', 100.0f, '%', 600.0f, '%', 'l', 't', 'c', 'c', 'n', frame->theme);
        listFrame->flags &= ~NODE_VISIBLE;
        listFrame->flags &= ~NODE_ACTIVE;
        Frame *listFrameFrame = listFrame->object;
        listFrameFrame->theme = frame->theme;
        listFrameFrame->flags |= FRAME_BACKGROUND;
        listFrame->children = realloc(listFrame->children, sizeof(Node *));

        Node *list = malloc(sizeof(Node));
        POINTER_CHECK(list);
        list = new ControlFrame(2.0f, '%', 2.0f, '%', 96.0f, '%', 96.0f, '%', 'l', 't', 'c', 'c', 'n');
        Frame *listFrameControl = list->object;
        listFrameControl->theme = frame->theme;
        listFrameControl->flags |= FRAME_BACKGROUND;
        listFrameControl->flags |= OVERFLOW_SCROLL;
        list->children = realloc(list->children, sizeof(Node *) * selectList->count);

        listFrame::add_child(list);
        this::add_child(listFrame);

        for (int i = 0; i < selectList->count; i++) {

            Node *child = malloc(sizeof(Node));
            POINTER_CHECK(child);
            child = new ControlFrame(0.0f, '%', frame->size[1]*i, 'p', 100.0f, '%', frame->size[1], 'p', 'l', 't', 'c', 'c', 'n');
            Frame *childFrame = (Frame *) child->object;
            childFrame->theme = frame->theme;
            child->children = realloc(child->children, sizeof(Node *));
            list::add_child(child);

            Node *button = malloc(sizeof(Node));
            POINTER_CHECK(button);
            button = new Button();
            button::init_button();
            button->children = realloc(button->children, sizeof(Node *));

            Node *label = malloc(sizeof(Node));
            POINTER_CHECK(label);
            label = new Label(selectList->options[i], 'l', 'c');
            Frame *labelFrame = (Frame *) label->object;
            labelFrame->relPos[0] = 5.0f;

            child::add_child(button);
            button::add_child(label);
        }
    }

    /**
     * @brief Refreshes the select list.
     *
     * This function is responsible for refreshing the select list. It updates the
     * display and state of the select list to reflect any changes that have occurred.
     */
    void refresh() {
        SUPER(refresh);
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = (SelectList *) frame->selectList;

        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        frame->contentSurface = SDL_CreateRGBSurface(0,frame->size[0],frame->size[1],32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (!frame->contentSurface) {
            PRINT_ERROR("Error creating surface: %s\n", SDL_GetError());
        }
        if (selectList->selected && selectList->options[*selectList->selected][0]) draw_text(frame->contentSurface, 0, 0, selectList->options[*selectList->selected], frame->theme->font.font, frame->theme->textColor, frame->alignment, -1);
        else if (selectList->options[0][0]) draw_text(frame->contentSurface, 0, 0, selectList->options[0], frame->theme->font.font, (SDL_Color) {frame->theme->textColor.r, frame->theme->textColor.g, frame->theme->textColor.b, frame->theme->textColor.a*0.8}, frame->alignment, -1);
        SUPER(refreshContent);
    }

    /**
     * @brief Updates the state of the select list.
     *
     * This function is responsible for updating the state of the select list. It performs necessary
     * operations to ensure the select list is in the correct state based on the current context.
     *
     * @note This function does not take any parameters and does not return any value.
     */
    void update() {
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = (SelectList *) frame->selectList;
        Mouse *mouse = &Game.input->mouse;
        if (Game.renderTarget) {
            mouse = &Game.renderTarget->mouse;
        }
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
            if (Game.input->released_keys & KEY_VALIDATE || ((mouse->released_button == SDL_BUTTON_LEFT || mouse->pressed_button == SDL_BUTTON_LEFT) &&
                !(mouse->x > x &&
                mouse->x < x+w &&
                mouse->y > y &&
                mouse->y < y+h &&
                mouse->x > frame->overflow[0] &&
                mouse->x < frame->overflow[2] &&
                mouse->y > frame->overflow[1] &&
                mouse->y < frame->overflow[3]))) {
                    for (int i = 0; i < this->children[0]->children[0]->length; i++) {
                        Frame *childFrame = (Frame *) this->children[0]->children[0]->children[i]->children[0]->object;
                        Button *button = (Button *) childFrame->button;
                        if (button->state != BUTTON_STATE_NORMAL) {
                            if (selectList->selected) *selectList->selected = i;
                            break;
                        }
                    }
                    selectList->state = BUTTON_STATE_NORMAL;
                    this::refresh();
                }
        } else {
            if (this->children) {
                this->children[0]->flags &= ~NODE_VISIBLE;
                this->children[0]->flags &= ~NODE_ACTIVE;
            }
            if (mouse->x > x &&
                mouse->x < x+w &&
                mouse->y > y &&
                mouse->y < y+h &&
                mouse->x > frame->overflow[0] &&
                mouse->x < frame->overflow[2] &&
                mouse->y > frame->overflow[1] &&
                mouse->y < frame->overflow[3]) {
                
                selectList->state = BUTTON_STATE_HOVERED;

                if (mouse->pressed_button == SDL_BUTTON_LEFT) {
                    selectList->state = BUTTON_STATE_PRESSED;
                }

            } else {
                selectList->state = BUTTON_STATE_NORMAL;
            }
        }
    }

    /**
     * @brief Checks if the current list is a select list.
     *
     * This function sets the value of the provided boolean pointer to indicate
     * whether the current list is a select list.
     *
     * @param result A pointer to a boolean variable where the result will be stored.
     *               The value will be set to true if the current list is a select list,
     *               otherwise it will be set to false.
     */
    void is_selectlist(bool *result) {
        *result = true;
    }

    /**
     * @brief Saves the current state of the Node editor to a file.
     *
     * This function writes the current state of the Node editor to the specified file.
     *
     * @param file A pointer to the FILE object where the state will be saved.
     * @param editor A pointer to the Node editor whose state is to be saved.
     */
    void save(FILE *file, Node *editor) {
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    /**
     * @brief Frees the resources allocated by the select list class.
     *
     * This function is responsible for releasing any memory or resources
     * that were allocated for the select list class. It ensures that there
     * are no memory leaks by properly deallocating all used resources.
     */
    void free() {
        Frame *frame = (Frame *) this->object;
        SelectList *selectList = frame->selectList;
        free(selectList);
        SUPER(free);
    }
 
    
}

