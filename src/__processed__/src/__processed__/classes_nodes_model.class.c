#line 1 "src/__processed__/classes_nodes_model.class.c"
#include <stdarg.h>
#include "../types.h"
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
void __class_method_model_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        Model *model;
        model = malloc(sizeof(Model));
        if (file) {
            char path[100];
            fscanf(file,"(%100[^)])", path);
            load_obj_model(path, &model->data);
        } else {
            model->data = NULL;
        }
        this->type = __type__;
        call_method_0(METHOD(constructor,this,model));
}

#line 37 "src/classes/nodes/model.class.c"
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

#line 48 "src/classes/nodes/model.class.c"
void __class_method_model_precompile_display_lists(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Model *model = (Model *) this->object;
        ModelData *data = model->data;
        if (data->objects[0].displayLists[0]) return;
        #ifdef DEBUG
            printf("Precompiling display lists...\n");
        #endif
        for (int j = 0; j < data->length; j++) {
            u32 objectPosition = 0;
            glBindVertexArray(data->objects[j].VAO);
            for (int k = 0; k < data->objects[j].materialsCount; k++) {
                data->objects[j].displayLists[k] = glGenLists(1);
                glNewList(data->objects[j].displayLists[k], GL_COMPILE);
                glDrawArrays(GL_TRIANGLES, objectPosition, data->objects[j].materialsLength[k] * 3);
                objectPosition += data->objects[j].materialsLength[k] * 3;
                glEndList();
            }
        }
        glBindVertexArray(0);
}

#line 69 "src/classes/nodes/model.class.c"
void __class_method_model_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );(void)this;
        Model *model = (Model *) this->object;
        ModelData *data = model->data;

        if (!data) return;

        call_method_0(METHOD(precompile_display_lists,this));

        int modelLoc = glGetUniformLocation(activeShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, *modelMatrix);
        for (int j = 0; j < data->length; j++) {
            for (int k = 0; k < data->objects[j].materialsCount; k++) {
                glActiveTexture(GL_TEXTURE0);

                glUniform3fv(glGetUniformLocation(activeShader, "material.ambient"), 1, data->objects[j].materials[k]->flatColors[AMBIENT_MATERIAL_PROPERTY]);
                glUniform3fv(glGetUniformLocation(activeShader, "material.specular"), 1, data->objects[j].materials[k]->flatColors[SPECULAR_MATERIAL_PROPERTY]);
                glUniform3fv(glGetUniformLocation(activeShader, "material.diffuse"), 1, data->objects[j].materials[k]->flatColors[DIFFUSE_MATERIAL_PROPERTY]);
                glUniform1fv(glGetUniformLocation(activeShader, "material.parallax"), 1, data->objects[j].materials[k]->flatColors[PARALLAX_MATERIAL_PROPERTY]);
                glUniform1fv(glGetUniformLocation(activeShader, "material.shininess"), 1, &data->objects[j].materials[k]->specularExp);
                if (data->objects[j].materials[k]->textureMaps[DIFFUSE_MATERIAL_PROPERTY]) {
                    set_shader_int(activeShader, "diffuseMapActive", 1); 
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, data->objects[j].materials[k]->textureMaps[DIFFUSE_MATERIAL_PROPERTY]);
                } else {
                    set_shader_int(activeShader, "diffuseMapActive", 0);
                }
                if (data->objects[j].materials[k]->textureMaps[NORMAL_MATERIAL_PROPERTY]) {
                    set_shader_int(activeShader, "normalMapActive", 1);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, data->objects[j].materials[k]->textureMaps[NORMAL_MATERIAL_PROPERTY]);
                } else {
                    set_shader_int(activeShader, "normalMapActive", 0);
                }
                if (data->objects[j].materials[k]->textureMaps[PARALLAX_MATERIAL_PROPERTY]) {
                    set_shader_int(activeShader, "parallaxMapActive", 1);
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, data->objects[j].materials[k]->textureMaps[PARALLAX_MATERIAL_PROPERTY]);
                } else {
                    set_shader_int(activeShader, "parallaxMapActive", 0);
                }
                
                glCallList(data->objects[j].displayLists[k]);
                
            }
        }

        if (model->flags & MODEL_FLAG_GLOW) {
            Shader glowShader;
            call_method_0(METHOD(get_glow_shader,this,&glowShader));
            use_shader(glowShader);

            int modelLoc = glGetUniformLocation(glowShader, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, *modelMatrix);
            set_shader_float(glowShader, "outlineWidth", 1.0f/this->globalScale[0]);
                        
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            for (int j = 0; j < data->length; j++) {
                for (int k = 0; k < data->objects[j].materialsCount; k++) {
                    glCallList(data->objects[j].displayLists[k]);
                }
            }
            glDisable(GL_CULL_FACE);
        }


}

#line 137 "src/classes/nodes/model.class.c"
void __class_method_model_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        // See src/memory.c for the implementation of free_models
        for (int i = 0; i < this->length; i++) {
            call_method_0(METHOD(free,(this->children[i])));
        }
        free(this->attribute);
        free(this->children);
        free(this);
}
    
