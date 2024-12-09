#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "memory.h"

class Model @promote extends Node {
    __containerType__ Node *

    void constructor(struct Model *model) {
        this->object = model;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        Model *model;
        if (file) {
            char path[100];
            fscanf(file,"(%100[^)])", path);
            load_obj_model(path, &model);
        }
        METHOD_TYPE(this, CLASS_TYPE_MODEL, constructor, model);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        Model *model = (Model*) this->object;
        for (int i = 0; i < memoryCaches.modelsCount; i++) {
            if (memoryCaches.modelCache[i].model == model) {
                fprintf(file, "(%s)", memoryCaches.modelCache[i].modelName);
                break;
            }
        }
    }

    void render(mat4 *modelMatrix) {
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

    void free() {
        for (int i = 0; i < this->length; i++) {
            METHOD(this->children[i], free);
        }
        free(this->params);
        free(this->children);
        free(this);
    }
    
}
