#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "memory.h"

class TexturedMesh @promote extends Node {
    __containerType__ Node *

    void constructor(struct TexturedMesh *texturedMesh) {
        this->object = texturedMesh;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        TexturedMesh *texturedMesh;
        texturedMesh = malloc(sizeof(TexturedMesh));
        POINTER_CHECK(texturedMesh);
        char path[100];
        if (file) {
            fscanf(file,"(%100[^)])", path);
        } else {
            path[0] = 0;
        }
        create_textured_plane(texturedMesh, path);
        METHOD_TYPE(this, CLASS_TYPE_TEXTUREDMESH, constructor, texturedMesh);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        TextureMap texture = ((TexturedMesh*) this->object)->texture;
        for (int i = 0; i < memoryCaches.texturesCount; i++) {
            if (memoryCaches.textureCache[i].textureMap == texture) {
                fprintf(file, "(%s)", memoryCaches.textureCache[i].textureName);
                break;
            }
        }
    }


    void render(mat4 *modelMatrix) {
        Shader shader;
        glGetIntegerv(GL_CURRENT_PROGRAM, (int*) &shader);

        vec3 defaultColor = {0.5f, 0.5f, 0.5f};
        glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, &defaultColor);
        glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, &defaultColor);
        glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, &defaultColor);
        glUniform1f(glGetUniformLocation(shader, "material.parallax"), 1, 0.5f);

        set_shader_int(shader, "diffuseMapActive", 1);
        int modelLoc = glGetUniformLocation(shader, "model");
        TexturedMesh *texturedMesh = (TexturedMesh *)this->object;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        // render Cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturedMesh->texture);
        glBindVertexArray(texturedMesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, texturedMesh->length);
        glBindVertexArray(0);
    }

    
}
