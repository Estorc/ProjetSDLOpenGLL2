/**
 * @file
 * @brief This file contains the implementation of the model class.
 * 
 * This source file contains the implementation of the model class, which is used
 * to represent and manage 3D models in the application. It includes functions for
 * loading, rendering, and manipulating 3D models.
 * 
 * @author Loup Picault
 * @date October 11, 2023
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "memory.h"
#include "render/render.h"

/**
 * @ingroup Classes Classes
 * @{
 */

class Model : public Node {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the model class.
     *
     * This function initializes a model object using the provided file path.
     *
     * @param path The file path to the model data.
     */
    void constructor(const char *path) {
        this->type = __type__;

        Model *model;
        model = malloc(sizeof(Model));
        load_model(path, &model->data);

        this->object = model;
        SUPER(initialize_node);
        if (model) {
            model->flags = MODEL_FLAG_NONE;
        }
    }

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the provided file pointer and loads it into the appropriate data structures.
     *
     * @param file A pointer to the file to be read.
     */
    void load(FILE *file) {

        char path[100] = "";
        if (file) {
            fscanf(file,"(%100[^)])", path);
        }
        this::constructor(path);
    }

    /**
     * @brief Retrieves settings data.
     *
     * This function retrieves settings data and stores it in the provided pointer.
     *
     * @param[out] ptr A pointer to a pointer to a pointer where the settings data will be stored.
     * @param[out] length A pointer to an integer where the length of the settings data will be stored.
     */
    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        Model *model = (Model *) this->object;
        void *data[] = {
            "model", "Model : ", &model->data,
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    /**
     * @brief Saves the current state of the model to a file.
     *
     * This function writes the current state of the model to the specified file.
     *
     * @param file A pointer to the FILE object where the model state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        Model *model = this->object;
        for (int i = 0; i < Game.memoryCaches->modelsCount; i++) {
            if (Game.memoryCaches->modelCache[i].model == model->data) {
                fprintf(file, "(%s)", Game.memoryCaches->modelCache[i].modelName);
                break;
            }
        }
    }

    /**
     * @brief Renders the model using the provided model matrix and active shader.
     *
     * This function takes a model matrix and an active shader as parameters and
     * renders the model accordingly. The model matrix is used to transform the
     * model's vertices, and the active shader is used to apply shading effects
     * during rendering.
     *
     * @param modelMatrix Mat4 structure representing the model matrix.
     * @param activeShader The shader to be used for rendering the model.
     */
    void render(mat4 modelMatrix, Shader activeShader) {

        render_model(modelMatrix, activeShader, this->object);

    }

    /**
     * @brief Frees the resources allocated by the model class.
     *
     * This function is responsible for releasing any memory or resources
     * that were allocated for the model class.
     */
    void free() {
        // See src/memory.c for the implementation of free_models
        for (int i = 0; i < this->length; i++) {
            (this->children[i])::free();
        }
        free(this->attribute);
        free(this->children);
        free(this);
    }
    
}

