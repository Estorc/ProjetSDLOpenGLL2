/**
 * @file
 * @brief This file defines the implementation of frame-related functionalities.
 *
 * This file contains the implementation of various functions and structures
 * that are used to manage and manipulate frames within the application.
 * It is used in the context of handling graphical frames in an SDL and OpenGL
 * based project.
 *
 * @author Loup Picault
 * @date 2023-10-03
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
#include <memory.h>

/**
 * @ingroup Classes Classes
 * @{
 */
class Frame : public Node {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for initializing a frame object with specified parameters.
     *
     * @param x The x-coordinate of the frame.
     * @param xu The unit for the x-coordinate.
     * @param y The y-coordinate of the frame.
     * @param yu The unit for the y-coordinate.
     * @param w The width of the frame.
     * @param wu The unit for the width.
     * @param h The height of the frame.
     * @param hu The unit for the height.
     * @param ha The horizontal alignment of the frame.
     * @param va The vertical alignment of the frame.
     * @param scroll The scroll setting for the frame.
     * @param theme A pointer to the Theme object associated with the frame.
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

        frame->flags |= FRAME_BACKGROUND;
        if (scroll == 's') {
            frame->flags |= OVERFLOW_SCROLL;
        } else if (scroll == 'v') {
            frame->flags |= OVERFLOW_VISIBLE;
        }

        frame->theme = theme;
    }

    /**
     * @brief Handles the conversion of dimension units for a given source and destination.
     *
     * This function calculates the dimensions of a graphical element based on the provided unit type and container dimensions.
     * It should be called every frame to ensure the dimensions are updated correctly.
     *
     * @param src Pointer to the source dimension value.
     * @param dest Pointer to the destination dimension value where the result will be stored.
     * @param vertical Integer flag indicating if the dimension is vertical (1 for vertical, 0 for horizontal).
     * @param size The size value to be converted.
     * @param unit The unit type for the size (e.g., pixels, percentage).
     * @param containerWidth The width of the container in which the element is placed.
     * @param containerHeight The height of the container in which the element is placed.
     */
    void handle_dimension_unit(float *src, float *dest, int vertical, double size, int unit, double containerWidth, double containerHeight) { // Devrait être calculé chaque frame
        *dest = *src;
        *dest = (vertical && size) ? -*dest : *dest; // Inverser l'axe vertical car OpenGL est en coordonnées inversées
        float axisSize = (vertical) ? containerHeight : containerWidth;
        if (size) *dest *= 2;
        switch (unit) {
        case 'p':
            *dest /= (vertical) ? containerHeight : containerWidth;
            break;
        case 'w':
            *dest /= 1.0f/(((1.0f/axisSize)*containerWidth)/100.0f); // Doit prendre en compte la taille de l'écran
            break;
        case 'h':
            *dest /= 1.0f/(((1.0f/axisSize)*containerHeight)/100.0f); // Doit prendre en compte la taille de l'écran
            break;
        case '%':
            *dest /= 100.0f;
            break;
        }
    }

    /**
     * @brief Initializes the frame.
     *
     * This function sets up the initial state of the frame. It should be called
     * before using any other functions related to the frame.
     */
    void init_frame() {
        Frame *frame = (Frame *) this->object;
        frame->alignment[0] = 'l';
        frame->alignment[1] = 't';
        frame->flags = 0;
        frame->flags |= FRAME_VISIBLE;
        frame->flags |= FRAME_NEEDS_REFRESH;
        frame->flags |= FRAME_NEEDS_INIT;
        frame->contentTexture = 0;
        frame->contentSurface = NULL;
        frame->theme = NULL;
        frame->absPos[0] = 0.0f;
        frame->absPos[1] = 0.0f;
        frame->size[0] = 0.0f;
        frame->size[1] = 0.0f;
        glm_vec2_zero(frame->contentSize);
        glm_vec2_zero(frame->scroll);
        glm_vec2_zero(frame->scrollTarget);
    }

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the given file and processes it accordingly.
     *
     * @param file A pointer to the FILE object that represents the file to be read.
     */
    void load(FILE *file) {
        float relPos[2], scale[2];
        char unit[4], alignment[2], scroll;
        char texturePath[256];
        Theme *theme = NULL;
        if (file) {
            fscanf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c%c", 
                &relPos[0],&unit[0], 
                &relPos[1],&unit[1], 
                &scale[0],&unit[2], 
                &scale[1],&unit[3], 
                &alignment[0], &alignment[1],
                &scroll);
            char c = fgetc(file);
            if (c == ',') {
                theme = malloc(sizeof(Theme));
                fscanf(file, "[%[^,],%[^,],%d,#%2hhx%2hhx%2hhx%2hhx])",	 
                    texturePath,
                    theme->font.path,
                    &theme->font.size,
                    &theme->textColor.r, &theme->textColor.g, &theme->textColor.b, &theme->textColor.a);
                theme->font.font = TTF_OpenFont(theme->font.path, theme->font.size);
                theme->windowSkin = load_texture_from_path(texturePath, GL_SRGB_ALPHA, true);
            }
        }
        this::constructor(relPos[0], unit[0], relPos[1], unit[1], scale[0], unit[2], scale[1], unit[3], alignment[0], alignment[1], scroll, theme);
    }

    /**
     * @brief Refreshes the frame.
     *
     * This function is responsible for updating the frame. It performs necessary
     * operations to refresh the frame's state.
     */
    void refresh() {

        int window_width, window_height;
        get_resolution(&window_width, &window_height);
        if (Game.renderTarget) {
            window_width = Game.renderTarget->w;
            window_height = Game.renderTarget->h;
        }
        vec2 containerSize = {window_width, window_height};
        Frame *frame = (Frame *) this->object;
        vec2 scroll = {0.0f, 0.0f};

        if (frame->flags & OVERFLOW_SCROLL) {
            frame->contentSize[0] = 0.0f;
            frame->contentSize[1] = 0.0f;
        }

        if (frame->theme && frame->theme->parent == frame) {
            TTF_CloseFont(frame->theme->font.font);
            frame->theme->font.font = TTF_OpenFont(frame->theme->font.path, frame->theme->font.size * ((float) window_width / 800.0f) * (1.33 / ((float) window_width / window_height)));
        }

        glm_vec4_copy((vec4) {0.0f, 0.0f, containerSize[0], containerSize[1]}, frame->overflow);

        for (int i = 0; i < this->length; i++) {
            bool is_child_gui = false;
            (this->children[i])::is_gui_element(&is_child_gui);
            if (is_child_gui) {
                Frame * childFrame = (Frame *) this->children[i]->object;
                childFrame->flags |= FRAME_NEEDS_REFRESH;
            }
        }

        bool is_parent_gui = false;
        if (this->parent) {
            (this->parent)::is_gui_element(&is_parent_gui);
        }
        if (is_parent_gui) {
            Frame * parentFrame = (Frame *) this->parent->object;
            if (!frame->theme) frame->theme = parentFrame->theme;
            containerSize[0] = parentFrame->size[0];
            containerSize[1] = parentFrame->size[1];
            if (!(parentFrame->flags & OVERFLOW_VISIBLE)) {
                glm_vec2_copy(parentFrame->scroll, scroll);
                glm_vec4_copy(parentFrame->overflow, frame->overflow);

                vec4 overflow;
                overflow[0] = parentFrame->absPos[0];
                overflow[1] = parentFrame->absPos[1];
                overflow[2] = parentFrame->size[0] + parentFrame->absPos[0];
                overflow[3] = parentFrame->size[1] + parentFrame->absPos[1];

                if (frame->overflow[0] < overflow[0]) frame->overflow[0] = overflow[0];
                if (frame->overflow[1] < overflow[1]) frame->overflow[1] = overflow[1];
                if (frame->overflow[2] > overflow[2]) frame->overflow[2] = overflow[2];
                if (frame->overflow[3] > overflow[3]) frame->overflow[3] = overflow[3];
            }
        }

        if (frame->unit[2] != '!') this::handle_dimension_unit(&frame->scale[0], &this->scale[0], false, 0.0f, frame->unit[2], (double) containerSize[0], (double) containerSize[1]);
        if (frame->unit[3] != '!') this::handle_dimension_unit(&frame->scale[1], &this->scale[1], true, 0.0f, frame->unit[3], (double) containerSize[0], (double) containerSize[1]);
        if (frame->unit[2] == 'a') {
            float scale = this->scale[1] * containerSize[1] / frame->contentSize[0];
            this::handle_dimension_unit(&scale, &this->scale[0], false, 0.0f, 'p', (double) containerSize[0], (double) containerSize[1]);
        }
        if (frame->unit[3] == 'a') {
            float scale = ((this->scale[0] * containerSize[0]) / frame->contentSize[0]) * frame->contentSize[1];
            this::handle_dimension_unit(&scale, &this->scale[1], true, 0.0f, 'p', (double) containerSize[0], (double) containerSize[1]);
            printf("Scale: %f\n", this->scale[1]);
        }
        if (frame->unit[0] != '!') this::handle_dimension_unit(&frame->relPos[0], &this->pos[0], false, this->scale[0], frame->unit[0], (double) containerSize[0], (double) containerSize[1]);
        if (frame->unit[1] != '!') this::handle_dimension_unit(&frame->relPos[1], &this->pos[1], true, this->scale[1], frame->unit[1], (double) containerSize[0], (double) containerSize[1]);
        glm_vec2_add(this->pos, scroll, this->pos);

        frame->absPos[0] = this->pos[0];
        frame->absPos[1] = -this->pos[1]; // Inverser l'axe vertical car OpenGL est en coordonnées inversées

        switch (frame->alignment[0]) {
            case 'l':
                this->pos[0] += -1.0f + this->scale[0];
                break;
            case 'c':
                frame->absPos[0] += 1.0f-this->scale[0];
                break;
            case 'r':
                frame->absPos[0] += 2.0f-this->scale[0]*2.0f;
                this->pos[0] += 1.0f - this->scale[0];
                break;
        }

        switch (frame->alignment[1]) {
            case 'b':
                frame->absPos[1] += 2.0f-this->scale[1]*2.0f;
                this->pos[1] += -1 + this->scale[1];
                break;
            case 'c':
                frame->absPos[1] += 1.0f-this->scale[1];
                break;
            case 't':
                this->pos[1] += 1 - this->scale[1];
                break;
        }

        frame->absPos[0] *= containerSize[0]/2.0f;
        frame->absPos[1] *= containerSize[1]/2.0f;


        frame->size[0] = this->scale[0]*containerSize[0];
        frame->size[1] = this->scale[1]*containerSize[1];
        if (is_parent_gui) {
            Frame * parentFrame = (Frame *) this->parent->object;
            frame->absPos[0] += parentFrame->absPos[0];
            frame->absPos[1] += parentFrame->absPos[1];
            if (parentFrame->contentSize[0] < this->scale[0] + this->pos[0] - scroll[0]) parentFrame->contentSize[0] = this->scale[0] + this->pos[0] - scroll[0];
            if (parentFrame->contentSize[1] < this->scale[1] - this->pos[1] + scroll[1]) parentFrame->contentSize[1] = this->scale[1] - this->pos[1] + scroll[1];
        }
        
        frame->flags &= ~FRAME_NEEDS_REFRESH;
    }

    /**
     * @brief Refreshes the content of the frame.
     *
     * This function is responsible for updating or refreshing the content displayed within the frame.
     * It ensures that the frame's content is up-to-date and reflects any changes that may have occurred.
     */
    void refreshContent() {
        Frame *frame = (Frame *) this->object;
        glBindTexture(GL_TEXTURE_2D, frame->contentTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, frame->contentSurface->w, frame->contentSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->contentSurface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /**
     * @brief Updates the state of the frame.
     *
     * This function is responsible for updating the internal state of the frame.
     * It should be called regularly to ensure the frame's state is kept up-to-date.
     */
    void update() {
        Frame *frame = (Frame *) this->object;
        if (frame->flags & OVERFLOW_SCROLL) {
            frame->scrollTarget[1] -= Game.input->mouse.scroll_y/8.0f;
            if (frame->scrollTarget[1] < 0.0f) frame->scrollTarget[1] = 0.0f;
            if (frame->scrollTarget[1] > frame->contentSize[1] - 1.0f) frame->scrollTarget[1] = frame->contentSize[1] - 1.0f;
            frame->scroll[1] += (frame->scrollTarget[1] - frame->scroll[1]) * 0.5f;
            if (fabs(frame->scrollTarget[1] - frame->scroll[1]) <= 0.01f) frame->scroll[1] = frame->scrollTarget[1]; // Prevents never-ending scrolling
            if (Game.input->mouse.scroll_y || frame->scrollTarget[1] != frame->scroll[1]) frame->flags |= FRAME_NEEDS_REFRESH;
        }
    }

    /**
     * @brief Handles the resize event for the frame.
     *
     * This function is called when the frame is resized. It updates the frame's
     * dimensions and performs any necessary adjustments to ensure the frame
     * is displayed correctly.
     */
    void on_resize() {
        Frame *frame = (Frame *) this->object;
        frame->flags |= FRAME_NEEDS_REFRESH;
    }

    /**
     * @brief Draws a frame.
     *
     * This function is responsible for rendering a frame. The specific details of
     * how the frame is drawn are not provided in the given code snippet.
     */
    void draw_frame() {
        VAO vao;
        this::get_vao(&vao);
        glBindVertexArray(vao);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glBindVertexArray(0);
    }

    /**
     * @var _vbo
     * @brief Static variable to store the Vertex Buffer Object (VBO) identifier.
     *
     * This static variable is used to store the identifier of the VBO that is
     * used for rendering frames. It is initialized to 0, indicating that no VBO
     * has been created or assigned yet.
     */
    static VBO _vbo = 0;

    /**
     * @brief Static variable to store the Vertex Array Object (VAO) identifier.
     *
     * This static variable `_vao` is used to store the identifier of the Vertex Array Object (VAO)
     * associated with the frame. The VAO is used in OpenGL to store the state needed to supply
     * vertex data to the graphics pipeline.
     *
     * @note The initial value of `_vao` is set to 0, indicating that no VAO has been created yet.
     */
    static VAO _vao = 0;

    /**
     * @brief Renders a frame using the provided model matrix and shader.
     *
     * This function is responsible for rendering a frame in the scene. It takes
     * a model matrix, an active shader, and a set of world shaders as parameters.
     *
     * @param modelMatrix Model matrix to be used for rendering.
     * @param activeShader The shader to be used for rendering the frame.
     * @param shaders A pointer to the WorldShaders structure containing the shaders
     *                used in the world.
     */
    void render(mat4 modelMatrix, Shader activeShader, WorldShaders *shaders) {
        Frame *frame = (Frame *) this->object;
        if (frame->flags & FRAME_NEEDS_REFRESH || frame->flags & FRAME_NEEDS_INIT) this::refresh();
        if (frame->flags & FRAME_VISIBLE && activeShader != shaders->depth && !(frame->flags & FRAME_NEEDS_INIT)) {
            UNUSED(activeShader);
            Frame *frame = (Frame *) this->object;
            use_shader(shaders->gui);

            set_shader_vec2(shaders->gui, "pixelSize", (vec2) {frame->size[0], frame->size[1]});

            bool is_parent_gui = false;
            if (this->parent) {
                (this->parent)::is_gui_element(&is_parent_gui);
            }

            set_shader_vec2(shaders->gui, "pixelPosition", frame->absPos);
            set_shader_vec4(shaders->gui, "overflow", frame->overflow);
            set_shader_float(shaders->gui, "time", SDL_GetTicks());
            
            set_shader_int(shaders->gui, "background", 0);
            set_shader_int(shaders->gui, "content", 1);

            set_shader_int(shaders->gui, "backgroundEnabled", frame->flags & FRAME_BACKGROUND);
            set_shader_int(shaders->gui, "contentEnabled", frame->flags & FRAME_CONTENT);


            bool is_checkbox = false;
            this::is_checkbox(&is_checkbox);
            set_shader_int(shaders->gui, "checked", 0);
            if (is_checkbox) {
                set_shader_int(shaders->gui, "isCheckBox", 1);
            } else set_shader_int(shaders->gui, "isCheckBox", 0);

            if (this::(bool)is_radiobutton()) {
                RadioButton *radiobutton = frame->radiobutton;
                set_shader_int(shaders->gui, "isRadioButton", 1);
                if (radiobutton->checked) set_shader_int(shaders->gui, "checked", !!(*radiobutton->checked & radiobutton->id));
            } else set_shader_int(shaders->gui, "isRadioButton", 0);


            ButtonState state = BUTTON_STATE_NORMAL;
            bool is_element_type = false;

            this::is_button(&is_element_type);
            if (is_element_type) {
                if (frame->button->checked) set_shader_int(shaders->gui, "checked", *frame->button->checked);
                state = frame->button->state;
            }

            this::is_input_area(&is_element_type);
            if (is_element_type) state = frame->inputArea->state;

            this::is_selectlist(&is_element_type);
            if (is_element_type) state = frame->selectList->state;

            set_shader_int(shaders->gui, "pressed", state == BUTTON_STATE_PRESSED);
            set_shader_int(shaders->gui, "hovered", state == BUTTON_STATE_HOVERED);

            if (frame->flags & FRAME_BACKGROUND) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, frame->theme->windowSkin);
            }
            if (frame->flags & FRAME_CONTENT) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, frame->contentTexture);
            }

            set_shader_mat4(shaders->gui, "model", modelMatrix);
            this::draw_frame();
            use_shader(activeShader);
        }
        frame->flags &= ~FRAME_NEEDS_INIT;
    }

    /**
     * @brief Saves the current state of the editor node to a file.
     *
     * This function writes the state of the given editor node to the specified file.
     *
     * @param file A pointer to the FILE object where the editor node state will be saved.
     * @param editor A pointer to the Node object representing the editor whose state is to be saved.
     */
    void save(FILE *file, Node *editor) {
        Frame *frame = (Frame *) this->object;
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);

        fprintf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c%c", 
            frame->relPos[0],frame->unit[0], 
            frame->relPos[1],frame->unit[1], 
            frame->scale[0],frame->unit[2], 
            frame->scale[1],frame->unit[3], 
            frame->alignment[0], frame->alignment[1],
            (frame->flags & OVERFLOW_SCROLL) ? 's' : (frame->flags & OVERFLOW_VISIBLE) ? 'v' : 'n');
        if (frame->theme && frame->theme->parent == frame) {
            fprintf(file, ",[");
            TextureMap texture = frame->theme->windowSkin;
            for (int i = 0; i < Game.memoryCaches->texturesCount; i++) {
                if (Game.memoryCaches->textureCache[i].textureMap == texture) {
                    fprintf(file, "%s,", Game.memoryCaches->textureCache[i].textureName);
                    break;
                }
            }
            fprintf(file, "%s,%d,#%2hhx%2hhx%2hhx%2hhx])",	 
                frame->theme->font.path,
                frame->theme->font.size,
                frame->theme->textColor.r, frame->theme->textColor.g, frame->theme->textColor.b, frame->theme->textColor.a);
        } else {
            fprintf(file, ")");
        }
    }

    /**
     * @brief Retrieves the Vertex Array Object (VAO).
     *
     * This function is responsible for obtaining the VAO and storing it in the provided VAO structure.
     *
     * @param vao A pointer to a VAO structure where the retrieved VAO will be stored.
     */
    void get_vao(VAO *vao) {
        if (!_vao) {
            float quadVertices[] = {
                // positions        // texture Coords
                1.0f,  -1.0f,       0.0f,   1.0f,
                1.0f,   1.0f,       0.0f,   0.0f,
                -1.0f,   1.0f,       1.0f,   0.0f,

                1.0f,  -1.0f,       0.0f,   1.0f,
                -1.0f,   1.0f,       1.0f,   0.0f,
                -1.0f,  -1.0f,       1.0f,   1.0f
            };
            // setup plane VAO
            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &_vbo);
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        }
        *vao = _vao;
    }


    /**
     * @brief Determines if the current object is a GUI element.
     *
     * This function sets the value pointed to by the result parameter to indicate
     * whether the current object is a graphical user interface (GUI) element.
     *
     * @param result A pointer to a boolean variable where the result will be stored.
     *               The value will be set to true if the current object is a GUI element,
     *               and false otherwise.
     */
    void is_gui_element(bool *result) {
        *result = true;
    }

    /**
     * @brief Determines if a button is pressed.
     *
     * This function checks the state of a button and sets the result accordingly.
     *
     * @param[out] result Pointer to a boolean variable where the result will be stored.
     *                    - true: if the button is pressed.
     *                    - false: if the button is not pressed.
     */
    void is_button(bool *result) {
        *result = false;
    }

    /**
     * @brief Checks if the current area is an input area.
     *
     * This function determines whether the current area is designated as an input area.
     *
     * @param result A pointer to a boolean variable where the result will be stored.
     *               - true: The current area is an input area.
     *               - false: The current area is not an input area.
     */
    void is_input_area(bool *result) {
        *result = false;
    }

    /**
     * @brief Checks if the current item is in the selection list.
     *
     * This function sets the value of the provided boolean pointer to indicate
     * whether the current item is part of the selection list.
     *
     * @param result A pointer to a boolean variable where the result will be stored.
     *               The value will be set to true if the item is in the selection list,
     *               and false otherwise.
     */
    void is_selectlist(bool *result) {
        *result = false;
    }

    /**
     * @brief Checks if a condition is a checkbox.
     *
     * This function sets the value of the boolean pointer `result` to indicate
     * whether a certain condition is a checkbox.
     *
     * @param result A pointer to a boolean variable where the result will be stored.
     *               The value will be set to `true` if the condition is a checkbox,
     *               otherwise it will be set to `false`.
     */
    void is_checkbox(bool *result) {
        *result = false;
    }

    /**
     * @brief Checks if the current frame is a radio button.
     *
     * This function determines whether the current frame instance is a radio button.
     *
     * @return true if the frame is a radio button, false otherwise.
     */
    bool is_radiobutton() {
        return false;
    }

    /**
     * @brief Frees the resources allocated for the frame.
     *
     * This function is responsible for releasing any memory or resources
     * that were allocated for the frame. It should be called when the frame
     * is no longer needed to avoid memory leaks.
     */
    void free() {
        Frame *frame = (Frame *) this->object;
        if (frame->flags & FRAME_CONTENT) {
            glBindTexture(GL_TEXTURE_2D, 0);
            use_fbo(GL_FRAMEBUFFER, 0);
            if (frame->contentTexture) glDeleteTextures(1, &frame->contentTexture);
            if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        }
        if (frame->theme && frame->theme->parent == frame) {
            TTF_CloseFont(frame->theme->font.font);
            free(frame->theme);
        }
        SUPER(free);
    }
}

