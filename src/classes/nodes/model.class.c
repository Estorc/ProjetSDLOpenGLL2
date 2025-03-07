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

class Model : public Node {
    __containerType__ Node *
    public:

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

    void load(FILE *file) {

        char path[100] = "";
        if (file) {
            fscanf(file,"(%100[^)])", path);
        }
        this::constructor(path);
    }

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

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        Model *model = (Model*) this->object;
        for (int i = 0; i < Game.memoryCaches->modelsCount; i++) {
            if (Game.memoryCaches->modelCache[i].model == model->data) {
                fprintf(file, "(%s)", Game.memoryCaches->modelCache[i].modelName);
                break;
            }
        }
    }

    void render(mat4 *modelMatrix, Shader activeShader) {

        render_model(modelMatrix, activeShader, (Model *) this->object);

    }

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
