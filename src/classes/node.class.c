/**
 * @file node.class.c
 * @brief Implementation of the node class for the ProjetSDLOpenGLL2 project.
 *
 * This file contains the implementation details of the node class, including
 * its main functionalities, methods, and any associated data structures.
 * The node class is a fundamental component of the ProjetSDLOpenGLL2 project,
 * providing essential features and operations required by the project.
 *
 * @author Loup Picault
 * @date 2023-10-20
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

/**
 * @ingroup Classes Classes
 * @{
 */

class Node {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the Node class.
     * 
     * This function initializes a new instance of the Node class.
     * It sets up the necessary properties and states for the node.
     * 
     * @note Ensure that all required resources are available before calling this constructor.
     */
    void constructor() {
        this->type = __type__;
        this->object = NULL;
        this::initialize_node();
    }
 
    /**
     * @brief Initializes a node.
     *
     * This function sets up the necessary parameters and state for a node.
     * It should be called before using the node in any operations to ensure
     * it is properly initialized.
     */
    void initialize_node() {
        this->flags = NODE_DEFAULT_FLAGS;
        this->length = 0;
        this->children = NULL;
        this->behavior = NULL;
        this->attribute = NULL;
        this->attributes_count = 0;
        this->shader = 0;
        glm_vec3_zero(this->pos);
        glm_vec3_zero(this->rot);
        glm_vec3_one(this->scale);
        glm_vec3_zero(this->globalPos);
        glm_vec3_zero(this->globalRot);
        glm_vec3_one(this->globalScale);
    }

    /**
     * @var glowShader
     * @brief A static Shader variable used for glow effect.
     *
     * @details This static variable holds the shader program used to apply a glow effect
     * to the nodes. It is initialized to 0 and should be assigned a valid shader program
     * before use.
     */
    static Shader glowShader = 0;
    
    /**
     * @brief Retrieves the glow shader.
     * 
     * This function assigns the glow shader to the provided Shader object.
     * 
     * @param shader A pointer to a Shader object where the glow shader will be stored.
     */
    void get_glow_shader(Shader *shader) {
        if (!glowShader) {
            glowShader = create_shader("shaders/glow.vs", "shaders/glow.fs");
        }
        *shader = glowShader;
    }

    /**
     * @brief Retrieves settings data.
     * 
     * This function populates the provided pointer with settings data and sets the length of the data.
     * 
     * @param[out] ptr A pointer to a pointer to a pointer that will be set to the settings data.
     * @param[out] length A pointer to an integer that will be set to the length of the settings data.
     */
    void get_settings_data(void *** ptr, int * length) {

        if (!this->behavior) {
            PRINT_INFO("Behavior is null\n");
            this->behavior = realloc(this->behavior, sizeof(Behavior));     // Ensure that the behavior array is initialized
            memset(this->behavior, 0, sizeof(Behavior));                    // for editor use.
            this->flags |= NODE_SCRIPT;
        }

        void *data[] = {
            "vec3", "Position : ", &this->pos,
            "vec3", "Rotation : ", &this->rot,
            "vec3", "Scale : ", &this->scale,
            "node-flags", &(*this->behavior)[BEHAVIOR_SCRIPT_READY], &(*this->behavior)[BEHAVIOR_SCRIPT_UPDATE], &(*this->behavior)[BEHAVIOR_SCRIPT_SIGNAL]
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    /**
     * @brief Loads the necessary resources or data for the node.
     * 
     * This function is responsible for initializing and loading any resources,
     * data, or configurations required by the node. It should be called before
     * the node is used to ensure that all dependencies are properly set up.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Saves the current state to the specified file.
     * 
     * This function writes the current state of the object to the given file.
     * The file must be opened in write mode before calling this function.
     * 
     * @param file A pointer to a FILE object that identifies the file to which 
     * the state will be saved. The file should be opened in write mode.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    /**
     * @brief Renders a node using the specified model matrix and shader.
     * 
     * This function renders the specified node using the provided model matrix and active shader.
     * 
     * @param modelMatrix The model matrix to be used for rendering.
     * @param activeShader The shader to be used for rendering.
     */
    void prepare_render(mat4 modelMatrix, Shader activeShader) {
        
        set_shader_int(activeShader, "diffuseMapActive", 0);
        set_shader_int(activeShader, "normalMapActive", 0);
        set_shader_int(activeShader, "parallaxMapActive", 0);
        
        glm_translate(modelMatrix, (vec3){this->pos[0], this->pos[1], this->pos[2]});
        glm_rotate(modelMatrix, to_radians(this->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(modelMatrix, to_radians(this->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(modelMatrix, to_radians(this->rot[2]), (vec3){0.0f, 0.0f, 1.0f});
        glm_scale(modelMatrix, (vec3){this->scale[0], this->scale[1], this->scale[2]});
    }

    /**
     * @brief Renders the node.
     * 
     * This function is responsible for rendering the node. It should be called
     * whenever the node needs to be drawn on the screen. The specific rendering
     * implementation details are handled within this function.
     */
    void render() {
        //
    }

    /**
     * @brief Updates the global position of a node.
     *
     * This function updates the global position of a node based on the provided
     * position, rotation, and scale vectors. It modifies the input vectors to
     * reflect the new global position.
     *
     * @param pos Pointer to a vec3 structure representing the position vector.
     * @param rot Pointer to a vec3 structure representing the rotation vector.
     * @param scale Pointer to a vec3 structure representing the scale vector.
     */
    void update_global_position(vec3 pos, vec3 rot, vec3 scale) {
        vec3 nodePos;
        glm_vec3_copy(this->pos, nodePos);

        glm_vec3_mul(nodePos, scale, nodePos);

        mat4 rotationMatrix;
        vec3 radiansRot;
        glm_vec3_scale(rot, PI/180.0f, radiansRot);
        glm_euler(radiansRot, rotationMatrix);
        glm_vec3_rotate_m4(rotationMatrix, nodePos, nodePos);

        glm_vec3_add(pos, nodePos, pos);

        glm_vec3_add(rot, this->rot, rot);
        glm_vec3_mul(scale, this->scale, scale);
        glm_vec3_copy(pos, this->globalPos);
        glm_vec3_copy(rot, this->globalRot);
        glm_vec3_copy(scale, this->globalScale);
    }

    /**
     * @brief Updates the position, rotation, and scale of a node.
     * 
     * This function takes pointers to vec3 structures representing the position,
     * rotation, and scale of a node and updates them accordingly.
     * 
     * @param pos Pointer to a vec3 structure representing the position of the node.
     * @param rot Pointer to a vec3 structure representing the rotation of the node.
     * @param scale Pointer to a vec3 structure representing the scale of the node.
     */
    void update(vec3 pos, vec3 rot, vec3 scale) {
        this::update_global_position(pos, rot, scale);
    }

    /**
     * @brief Handles the resize event for the node.
     *
     * This function is called when the node needs to respond to a resize event.
     * It adjusts the node's properties and layout to accommodate the new size.
     */
    void on_resize() {
        //
    }

    /**
     * @brief Frees the resources allocated by the object.
     * 
     * This function is responsible for releasing any memory or resources
     * that were allocated by the object. It should be called when the object
     * is no longer needed to avoid memory leaks.
     */
    void free() {
        for (int i = 0; i < this->length; i++) {
            (this->children[i])::free();
        }
        free(this->object);
        free(this->attribute);
        free(this->children);
        free(this);
    }

    /**
     * @brief Checks if the current shape is a custom shape.
     * 
     * This function determines whether the current shape is a custom shape and 
     * sets the provided boolean pointer accordingly.
     * 
     * @param cshape A pointer to a boolean variable. The function sets this 
     *               variable to true if the current shape is a custom shape, 
     *               otherwise it sets it to false.
     */
    void is_cshape(bool *cshape) {
        *cshape = false;
    }

    /**
     * @brief Sets the body status of the node.
     * 
     * This function sets the body status of the node by modifying the value
     * pointed to by the provided boolean pointer.
     * 
     * @param body A pointer to a boolean variable that will be set to indicate
     *             the body status of the node. If the node is a body, the value
     *             pointed to by this parameter will be set to true; otherwise,
     *             it will be set to false.
     */
    void is_body(bool *body) {
        *body = false;
    }

    /**
     * @brief Checks if the current node is within a specified area.
     * 
     * This function determines whether the current node falls within a given area.
     * The result is stored in the boolean pointer provided as an argument.
     * 
     * @param area A pointer to a boolean variable where the result will be stored.
     *             The value will be set to true if the node is within the area,
     *             and false otherwise.
     */
    void is_area(bool *area) {
        *area = false;
    }

    /**
     * @brief Sets the render target status.
     * 
     * This function sets the status of whether the current node is a render target.
     * 
     * @param render_target A pointer to a boolean value. If true, the node is set as a render target.
     *                      If false, the node is not a render target.
     */
    void is_render_target(bool *render_target) {
        *render_target = false;
    }

    /**
     * @brief Determines if the current node is a GUI element.
     * 
     * This function checks whether the current node is a graphical user interface (GUI) element
     * and sets the result accordingly.
     * 
     * @param result A pointer to a boolean variable where the result will be stored.
     *               - true if the node is a GUI element.
     *               - false otherwise.
     */
    void is_gui_element(bool *result) {
        *result = false;
    } 

    /**
     * @brief Adds a child node to a parent node.
     * 
     * This function adds a child node to the specified parent node, establishing a parent-child
     * relationship in the scene graph.
     * 
     * @param child The child node to be added.
     */
    void add_child(Node *child) {
        
        this->children[this->length++] = child;
        child->parent = this;
        
    }

    /**
     * @brief Adds a child node to a parent node and reallocates memory if necessary.
     * 
     * This function adds a child node to the specified parent node and reallocates memory
     * for the parent's children array if needed to accommodate the new child.
     * 
     * @param child The child node to be added.
     */
    void add_child_and_realloc(Node *child) {
        this->children = realloc(this->children, sizeof(Node *) * (this->length+1));
        POINTER_CHECK(this->children);
        this->children[this->length++] = child;
        child->parent = this;
    }

    /**
     * @brief Removes a child node from a parent node.
     * 
     * This function removes a child node from the specified parent node, breaking the
     * parent-child relationship in the scene graph.
     * 
     * @param child The child node to be removed.
     */
    void remove_child(Node *child) {
        for (Node **rightCursor, **leftCursor = rightCursor = this->children; rightCursor < this->children + this->length; leftCursor++, rightCursor++) {
            if (child == *rightCursor) rightCursor++;
            if (leftCursor != rightCursor && rightCursor < this->children + this->length) *leftCursor = *rightCursor;
        }
    }

    /**
     * @brief Removes a child node from a parent node and reallocates memory if necessary.
     * 
     * This function removes a child node from the specified parent node and reallocates memory
     * for the parent's children array if needed to optimize memory usage.
     * 
     * @param child The child node to be removed.
     */
    void remove_child_and_realloc(Node *child) {
        for (Node **rightCursor, **leftCursor = rightCursor = this->children; rightCursor < this->children + this->length; leftCursor++, rightCursor++) {
            if (child == *rightCursor) rightCursor++;
            if (leftCursor != rightCursor && rightCursor < this->children + this->length) *leftCursor = *rightCursor;
        }
        this->children = realloc(this->children, sizeof(Node *) * (--this->length));
        //POINTER_CHECK(node->children);
    }

    /**
     * @brief Removes a child node from a parent node and frees the memory of the child node.
     * 
     * This function removes a child node from the specified parent node and frees the memory
     * allocated for the child node.
     * 
     * @param child The child node to be removed and freed.
     */
    void remove_child_and_free(Node *child) {
        for (Node **rightCursor, **leftCursor = rightCursor = this->children; rightCursor < this->children + this->length; leftCursor++, rightCursor++) {
            if (child == *rightCursor) (*rightCursor)::free(), rightCursor++;
            if (leftCursor != rightCursor && rightCursor < this->children + this->length) *leftCursor = *rightCursor;
        }
    }

    /**
     * @brief Removes a child node from a parent node, frees the memory of the child node, and reallocates memory if necessary.
     * 
     * This function removes a child node from the specified parent node, frees the memory
     * allocated for the child node, and reallocates memory for the parent's children array
     * if needed to optimize memory usage.
     * 
     * @param child The child node to be removed and freed.
     */
    void remove_child_and_free_and_realloc(Node *child) {
        for (Node **rightCursor, **leftCursor = rightCursor = this->children; rightCursor < this->children + this->length; leftCursor++, rightCursor++) {
            if (child == *rightCursor) {
                (*rightCursor)::free();
                rightCursor++;
            }
            if (leftCursor != rightCursor && rightCursor < this->children + this->length) *leftCursor = *rightCursor;
        }
        this->children = realloc(this->children, sizeof(Node *) * (--this->length));
        //POINTER_CHECK(node->children);
    }

    /**
     * @brief Retrieves the index of a child node in a parent node's children array.
     * 
     * This function retrieves the index of a child node in a parent node's children array.
     * 
     * @param child The child node whose index is to be retrieved.
     * @return The index of the child node in the parent node's children array.
     * 
     * @note If the child node is not found in the parent node's children array, the function returns -1.
     */
    int index_of_child(Node *child) {
        for (int i = 0; i < this->length; i++) {
            if (child == this->children[i]) return i;
        }
        return -1;
    }

    /**
     * @brief Prints the details of a node at a specified level of indentation.
     * 
     * This function prints the details of the specified node, including its properties and
     * children, at the specified level of indentation for better readability.
     * 
     * @param node The node to be printed.
     * @param level The level of indentation for printing.
     */
    void print(int level) {
        #ifdef DEBUG
        char tabs[level+1];
        for (int i = 0; i < level; i++) tabs[i] = '\t';
        tabs[level] = 0;
        PRINT_INFO("%s - [%s] (%s) %c%c%c%c%c %d children:\n", 
                tabs,
        
                classManager.class_names[this->type],
                "NodeName",

                (!level) ? 'r' : '_', 
                (this->flags & NODE_ACTIVE) ? 'a' : '_', 
                (this->flags & NODE_VISIBLE) ? 'v' : '_', 
                (this->flags & NODE_SCRIPT) ? 's' : '_', 
                (this->shader) ? 'S' : '_', 
                
                this->length);
        for (int i = 0; i < this->length; i++) {
            (this->children[i])::print(level+1);
        }
        #endif
    }

    /**
     * @brief Emits the 'ready' signal for the node.
     *
     * This function checks if the node has the NODE_SCRIPT flag set and if the 
     * BEHAVIOR_SCRIPT_READY behavior is defined. If both conditions are met, it 
     * calls the BEHAVIOR_SCRIPT_READY behavior function with the current node 
     * instance and the provided arguments.
     *
     * @param ... args The arguments to pass to the behavior function.
     *
     * @note The behavior function is expected to be a callable object stored in 
     * the behavior map with the key BEHAVIOR_SCRIPT_READY.
     */
    void emit_ready(...) {
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_READY]) (*this->behavior)[BEHAVIOR_SCRIPT_READY](this, args);
    }

    /**
     * @brief Emits an update event for the node.
     *
     * This function checks if the node has the NODE_SCRIPT flag set and if the 
     * behavior script for the update event is defined. If both conditions are met, 
     * it calls the update behavior script with the current node and provided arguments.
     *
     * @param ... args Arguments to be passed to the behavior script.
     */
    void emit_update(...) {
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_UPDATE]) (*this->behavior)[BEHAVIOR_SCRIPT_UPDATE](this, args);
    }

    /**
     * @brief Emits a signal if the node has a script and the script has a signal handler.
     * 
     * This function checks if the node has a script associated with it by verifying the NODE_SCRIPT flag.
     * If the node has a script and the script contains a signal handler for BEHAVIOR_SCRIPT_SIGNAL,
     * it calls the signal handler with the current node and the provided arguments.
     * 
     * @param ... args The arguments to be passed to the signal handler.
     */
    void emit_signal(...) {
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_SIGNAL]) (*this->behavior)[BEHAVIOR_SCRIPT_SIGNAL](this, args);
    }
}