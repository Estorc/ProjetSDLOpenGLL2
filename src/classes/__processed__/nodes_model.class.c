#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
#include "../../memory.h"
void __class_method_model_constructor(unsigned type, ...) {
unsigned __type__ = 16;
(void)__type__;
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
unsigned __type__ = 16;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_model_load(unsigned type, ...) {
unsigned __type__ = 16;
(void)__type__;
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
    }
    METHOD_TYPE(this, CLASS_TYPE_MODEL, constructor, model);
}

void __class_method_model_save(unsigned type, ...) {
unsigned __type__ = 16;
(void)__type__;
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

void __class_method_model_render(unsigned type, ...) {
unsigned __type__ = 16;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
mat4 * modelMatrix = va_arg(args, mat4 *);
va_end(args);
(void)this;
    Shader shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, (int*) &shader);
    
    int modelLoc = glGetUniformLocation(shader, "model");
    Model *model = (Model *) this->object;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, *modelMatrix);
    if (model) for (int j = 0; j < model->length; j++) {
        u32 objectPosition = 0;
        glBindVertexArray(model->objects[j].VAO);
        for (int k = 0; k < model->objects[j].materialsCount; k++) {
            glActiveTexture(GL_TEXTURE0);

            glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, &model->objects[j].materials[k]->flatColors[AMBIENT_MATERIAL_PROPERTY]);
            glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, &model->objects[j].materials[k]->flatColors[SPECULAR_MATERIAL_PROPERTY]);
            glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, &model->objects[j].materials[k]->flatColors[DIFFUSE_MATERIAL_PROPERTY]);
            glUniform1fv(glGetUniformLocation(shader, "material.parallax"), 1, &model->objects[j].materials[k]->flatColors[PARALLAX_MATERIAL_PROPERTY]);
            glUniform1fv(glGetUniformLocation(shader, "material.shininess"), 1, &model->objects[j].materials[k]->specularExp);
        if (model->objects[j].materials[k]->textureMaps[DIFFUSE_MATERIAL_PROPERTY]) {
            set_shader_int(shader, "diffuseMapActive", 1); 
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->textureMaps[DIFFUSE_MATERIAL_PROPERTY]);
        }
        if (model->objects[j].materials[k]->textureMaps[NORMAL_MATERIAL_PROPERTY]) {
            set_shader_int(shader, "normalMapActive", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->textureMaps[NORMAL_MATERIAL_PROPERTY]);
        }
        if (model->objects[j].materials[k]->textureMaps[PARALLAX_MATERIAL_PROPERTY]) {
            set_shader_int(shader, "parallaxMapActive", 1);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, model->objects[j].materials[k]->textureMaps[PARALLAX_MATERIAL_PROPERTY]);
        }
            
            glDrawArrays(GL_TRIANGLES, objectPosition, model->objects[j].materialsLength[k] * 3);
            
            objectPosition += model->objects[j].materialsLength[k] * 3;
            
        }
    }
    glBindVertexArray(0);
}

void __class_method_model_free(unsigned type, ...) {
unsigned __type__ = 16;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    for (int i = 0; i < this->length; i++) {
        METHOD(this->children[i], free);
    }
    free(this->params);
    free(this->children);
    free(this);
}

