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

#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Node {
    __containerType__ Node *
    public:

    void constructor() {
        this->object = NULL;
        this->type = __type__;
        this::initialize_node();
    }
 
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
    }

    static Shader glowShader = 0;
    void get_glow_shader(Shader *shader) {
        if (!glowShader) {
            glowShader = create_shader("shaders/glow.vs", "shaders/glow.fs");
        }
        *shader = glowShader;
    }

    

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

    void load() {
        this->type = __type__;
        this::constructor();
    }

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

    void prepare_render(mat4 *modelMatrix, Shader activeShader) {
        
        set_shader_int(activeShader, "diffuseMapActive", 0);
        set_shader_int(activeShader, "normalMapActive", 0);
        set_shader_int(activeShader, "parallaxMapActive", 0);
        
        glm_translate(*modelMatrix, (vec3){this->pos[0], this->pos[1], this->pos[2]});
        glm_rotate(*modelMatrix, to_radians(this->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(*modelMatrix, to_radians(this->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(*modelMatrix, to_radians(this->rot[2]), (vec3){0.0f, 0.0f, 1.0f});
        glm_scale(*modelMatrix, (vec3){this->scale[0], this->scale[1], this->scale[2]});
    }

    void render() {
        //
    }

    void update_global_position(vec3 *pos, vec3 *rot, vec3 *scale) {
        vec3 nodePos;
        glm_vec3_copy(this->pos, nodePos);

        glm_vec3_mul(nodePos, *scale, nodePos);

        glm_vec3_rotate(nodePos, to_radians((*rot)[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_vec3_rotate(nodePos, to_radians((*rot)[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_vec3_rotate(nodePos, to_radians((*rot)[2]), (vec3){0.0f, 0.0f, 1.0f});

        glm_vec3_add(*pos, nodePos, *pos);

        glm_vec3_add(*rot, this->rot, *rot);
        glm_vec3_mul(*scale, this->scale, *scale);
        glm_vec3_copy(*pos, this->globalPos);
        glm_vec3_copy(*rot, this->globalRot);
        glm_vec3_copy(*scale, this->globalScale);
    }

    void update(vec3 *pos, vec3 *rot, vec3 *scale) {
        this::update_global_position(pos, rot, scale);
    }

    void free() {
        for (int i = 0; i < this->length; i++) {
            (this->children[i])::free();
        }
        free(this->object);
        free(this->attribute);
        free(this->children);
        free(this);
    }

    void is_cshape(bool *cshape) {
        *cshape = false;
    }

    void is_body(bool *body) {
        *body = false;
    }

    void is_area(bool *area) {
        *area = false;
    }

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


    void emit_ready(...) {
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_READY]) (*this->behavior)[BEHAVIOR_SCRIPT_READY](this, args);
    }

    void emit_update(...) {
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_UPDATE]) (*this->behavior)[BEHAVIOR_SCRIPT_UPDATE](this, args);
    }

    void emit_signal(...) {
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_SIGNAL]) (*this->behavior)[BEHAVIOR_SCRIPT_SIGNAL](this, args);
    }
}