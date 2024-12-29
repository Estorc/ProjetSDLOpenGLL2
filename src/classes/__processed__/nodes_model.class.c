#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
#include "../../memory.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_MODEL;


void __class_method_model_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct Model * model = va_arg(args, struct Model *);
va_end(args);
(void)this;
    this->object = model;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_model_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_model_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    Model *model;
    if (file) {
        char path[100];
        fscanf(file,"(%100[^)])", path);
        load_obj_model(path, &model);
    } else {
        model = NULL;
    }
    METHOD_TYPE(this, __type__, constructor, model);
}


void __class_method_model_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    Model *model = (Model*) this->object;
    for (int i = 0; i < memoryCaches.modelsCount; i++) {
        if (memoryCaches.modelCache[i].model == model) {
            fprintf(file, "(%s)", memoryCaches.modelCache[i].modelName);
            break;
        }
    }
}


void __class_method_model_precompile_display_lists(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    Model *model = (Model *) this->object;
    if (model->objects[0].displayLists[0]) return;
    #ifdef DEBUG
        printf("Precompiling display lists...\n");
    #endif
    for (int j = 0; j < model->length; j++) {
        u32 objectPosition = 0;
        glBindVertexArray(model->objects[j].VAO);
        for (int k = 0; k < model->objects[j].materialsCount; k++) {
            model->objects[j].displayLists[k] = glGenLists(1);
            glNewList(model->objects[j].displayLists[k], GL_COMPILE);
            glDrawArrays(GL_TRIANGLES, objectPosition, model->objects[j].materialsLength[k] * 3);
            objectPosition += model->objects[j].materialsLength[k] * 3;
            glEndList();
        }
    }
    glBindVertexArray(0);
}


void __class_method_model_render(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
mat4 * modelMatrix = va_arg(args, mat4 *);
Shader  activeShader = va_arg(args, Shader );
va_end(args);
(void)this;
    int modelLoc = glGetUniformLocation(activeShader, "model");
    Model *model = (Model *) this->object;

    if (!model) return;

    METHOD(this, precompile_display_lists);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, *modelMatrix);
    for (int j = 0; j < model->length; j++) {
        for (int k = 0; k < model->objects[j].materialsCount; k++) {
            glActiveTexture(GL_TEXTURE0);

            glUniform3fv(glGetUniformLocation(activeShader, "material.ambient"), 1, model->objects[j].materials[k]->flatColors[AMBIENT_MATERIAL_PROPERTY]);
            glUniform3fv(glGetUniformLocation(activeShader, "material.specular"), 1, model->objects[j].materials[k]->flatColors[SPECULAR_MATERIAL_PROPERTY]);
            glUniform3fv(glGetUniformLocation(activeShader, "material.diffuse"), 1, model->objects[j].materials[k]->flatColors[DIFFUSE_MATERIAL_PROPERTY]);
            glUniform1fv(glGetUniformLocation(activeShader, "material.parallax"), 1, model->objects[j].materials[k]->flatColors[PARALLAX_MATERIAL_PROPERTY]);
            glUniform1fv(glGetUniformLocation(activeShader, "material.shininess"), 1, &model->objects[j].materials[k]->specularExp);
            if (model->objects[j].materials[k]->textureMaps[DIFFUSE_MATERIAL_PROPERTY]) {
                set_shader_int(activeShader, "diffuseMapActive", 1); 
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->textureMaps[DIFFUSE_MATERIAL_PROPERTY]);
            } else {
                set_shader_int(activeShader, "diffuseMapActive", 0);
            }
            if (model->objects[j].materials[k]->textureMaps[NORMAL_MATERIAL_PROPERTY]) {
                set_shader_int(activeShader, "normalMapActive", 1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->textureMaps[NORMAL_MATERIAL_PROPERTY]);
            } else {
                set_shader_int(activeShader, "normalMapActive", 0);
            }
            if (model->objects[j].materials[k]->textureMaps[PARALLAX_MATERIAL_PROPERTY]) {
                set_shader_int(activeShader, "parallaxMapActive", 1);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->textureMaps[PARALLAX_MATERIAL_PROPERTY]);
            } else {
                set_shader_int(activeShader, "parallaxMapActive", 0);
            }
            
            glCallList(model->objects[j].displayLists[k]);
            
        }
    }
}


void __class_method_model_free(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    // See src/memory.c for the implementation of free_models
    for (int i = 0; i < this->length; i++) {
        METHOD(this->children[i], free);
    }
    free(this->params);
    free(this->children);
    free(this);
}
    

