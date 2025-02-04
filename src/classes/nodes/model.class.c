#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "memory.h"
#include "render/render.h"

class Model : public Node {
    __containerType__ Node *
    public:

    void constructor(struct Model *model) {
        this->object = model;
        this->type = __type__;
        SUPER(initialize_node);
        if (model) {
            model->flags = MODEL_FLAG_NONE;
        }
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

    void load(FILE *file) {
        Model *model;
        model = malloc(sizeof(Model));
        if (file) {
            char path[100];
            fscanf(file,"(%100[^)])", path);
            load_model(path, &model->data);
        } else {
            model->data = NULL;
        }
        this->type = __type__;
        this::constructor(model);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        Model *model = (Model*) this->object;
        for (int i = 0; i < memoryCaches.modelsCount; i++) {
            if (memoryCaches.modelCache[i].model == model->data) {
                fprintf(file, "(%s)", memoryCaches.modelCache[i].modelName);
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
