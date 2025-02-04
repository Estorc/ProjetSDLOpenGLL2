#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_NODE;
#line 10 "src/classes/node.class.c"
void __class_method_node_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        this->object = NULL;
        this->type = __type__;
        this::initialize_node();
}
 
#line 16 "src/classes/node.class.c"
void __class_method_node_initialize_node(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
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
#line 30 "src/classes/node.class.c"
void __class_method_node_get_glow_shader(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Shader * shader = va_arg(args, Shader *);(void)this;
        if (!glowShader) {
            glowShader = create_shader("shaders/glow.vs", "shaders/glow.fs");
        }
        *shader = glowShader;
}

    

#line 39 "src/classes/node.class.c"
void __class_method_node_get_settings_data(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);void ***  ptr = va_arg(args, void *** );int *  length = va_arg(args, int * );(void)this;

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

#line 59 "src/classes/node.class.c"
void __class_method_node_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        this->type = __type__;
        this::constructor();
}

#line 64 "src/classes/node.class.c"
void __class_method_node_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
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

#line 78 "src/classes/node.class.c"
void __class_method_node_prepare_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );(void)this;
        
        set_shader_int(activeShader, "diffuseMapActive", 0);
        set_shader_int(activeShader, "normalMapActive", 0);
        set_shader_int(activeShader, "parallaxMapActive", 0);
        
        glm_translate(*modelMatrix, (vec3){this->pos[0], this->pos[1], this->pos[2]});
        glm_rotate(*modelMatrix, to_radians(this->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(*modelMatrix, to_radians(this->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(*modelMatrix, to_radians(this->rot[2]), (vec3){0.0f, 0.0f, 1.0f});
        glm_scale(*modelMatrix, (vec3){this->scale[0], this->scale[1], this->scale[2]});
}

#line 91 "src/classes/node.class.c"
void __class_method_node_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        //
}

#line 95 "src/classes/node.class.c"
void __class_method_node_update_global_position(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * pos = va_arg(args, vec3 *);vec3 * rot = va_arg(args, vec3 *);vec3 * scale = va_arg(args, vec3 *);(void)this;
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

#line 114 "src/classes/node.class.c"
void __class_method_node_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * pos = va_arg(args, vec3 *);vec3 * rot = va_arg(args, vec3 *);vec3 * scale = va_arg(args, vec3 *);(void)this;
        this::update_global_position(pos, rot, scale);
}

#line 118 "src/classes/node.class.c"
void __class_method_node_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        for (int i = 0; i < this->length; i++) {
            (this->children[i])::free();
        }
        free(this->object);
        free(this->attribute);
        free(this->children);
        free(this);
}

#line 128 "src/classes/node.class.c"
void __class_method_node_is_cshape(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * cshape = va_arg(args, bool *);(void)this;
        *cshape = false;
}

#line 132 "src/classes/node.class.c"
void __class_method_node_is_body(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * body = va_arg(args, bool *);(void)this;
        *body = false;
}

#line 136 "src/classes/node.class.c"
void __class_method_node_is_area(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * area = va_arg(args, bool *);(void)this;
        *area = false;
}

#line 140 "src/classes/node.class.c"
void __class_method_node_is_gui_element(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
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

#line 153 "src/classes/node.class.c"
void __class_method_node_add_child(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
        
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

#line 169 "src/classes/node.class.c"
void __class_method_node_add_child_and_realloc(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 185 "src/classes/node.class.c"
void __class_method_node_remove_child(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 201 "src/classes/node.class.c"
void __class_method_node_remove_child_and_realloc(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 219 "src/classes/node.class.c"
void __class_method_node_remove_child_and_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 236 "src/classes/node.class.c"
void __class_method_node_remove_child_and_free_and_realloc(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

int call_method_1(void (*func)(void *, va_list), ...) {
    va_list args;
    va_start(args, func);
    int value;
    func(&value, args);
    va_end(args);
    return value;
}
#line 259 "src/classes/node.class.c"
void __class_method_node_index_of_child(void * __retValueVP__, va_list args) {int * __retValueP__ = (int *) __retValueVP__;Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
        for (int i = 0; i < this->length; i++) {
            if (child == this->children[i]) *__retValueP__ = i;return;
        }
        *__retValueP__ = -1;return;
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

#line 276 "src/classes/node.class.c"
void __class_method_node_print(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);int  level = va_arg(args, int );(void)this;
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


#line 301 "src/classes/node.class.c"
void __class_method_node_emit_ready(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_READY]) (*this->behavior)[BEHAVIOR_SCRIPT_READY](this, args);
}

#line 305 "src/classes/node.class.c"
void __class_method_node_emit_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_UPDATE]) (*this->behavior)[BEHAVIOR_SCRIPT_UPDATE](this, args);
}

#line 309 "src/classes/node.class.c"
void __class_method_node_emit_signal(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        if (this->flags & NODE_SCRIPT && (*this->behavior)[BEHAVIOR_SCRIPT_SIGNAL]) (*this->behavior)[BEHAVIOR_SCRIPT_SIGNAL](this, args);
}
