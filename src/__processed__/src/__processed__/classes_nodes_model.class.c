#line 1 "src/__processed__/classes_nodes_model.class.c"
#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../memory.h"
#include "../render/render.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_MODEL;
#line 12 "src/classes/nodes/model.class.c"
void __class_method_model_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct Model * model = va_arg(args, struct Model *);(void)this;
        this->object = model;
        this->type = __type__;
        SUPER(initialize_node);
        if (model) {
            model->flags = MODEL_FLAG_NONE;
        }
}

    

#line 23 "src/classes/nodes/model.class.c"
void __class_method_model_get_settings_data(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);void ***  ptr = va_arg(args, void *** );int *  length = va_arg(args, int * );(void)this;
        SUPER(get_settings_data, ptr, length);
        Model *model = (Model *) this->object;
        void *data[] = {
            "model", "Model : ", &model->data,
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
}

#line 34 "src/classes/nodes/model.class.c"
void __class_method_model_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
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
        call_method_0(METHOD(constructor,this,model));
}

#line 48 "src/classes/nodes/model.class.c"
void __class_method_model_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
        Model *model = (Model*) this->object;
        for (int i = 0; i < memoryCaches.modelsCount; i++) {
            if (memoryCaches.modelCache[i].model == model->data) {
                fprintf(file, "(%s)", memoryCaches.modelCache[i].modelName);
                break;
            }
        }
}

#line 59 "src/classes/nodes/model.class.c"
void __class_method_model_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );(void)this;

        render_model(modelMatrix, activeShader, (Model *) this->object);

}

#line 65 "src/classes/nodes/model.class.c"
void __class_method_model_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        // See src/memory.c for the implementation of free_models
        for (int i = 0; i < this->length; i++) {
            call_method_0(METHOD(free,(this->children[i])));
        }
        free(this->attribute);
        free(this->children);
        free(this);
}
    
