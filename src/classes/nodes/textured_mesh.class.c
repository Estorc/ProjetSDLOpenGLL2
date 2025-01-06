#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "memory.h"

class TexturedMesh : public Node {
    __containerType__ Node *
    public:

    void constructor(struct TexturedMesh *texturedMesh) {
        this->object = texturedMesh;
        this->type = __type__;
        SUPER(initialize_node);
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
        this->type = __type__;
        this::constructor(texturedMesh);
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


    void render(mat4 *modelMatrix, Shader activeShader) {
        vec3 defaultColor = {0.5f, 0.5f, 0.5f};
        glUniform3fv(glGetUniformLocation(activeShader, "material.ambient"), 1, &defaultColor);
        glUniform3fv(glGetUniformLocation(activeShader, "material.specular"), 1, &defaultColor);
        glUniform3fv(glGetUniformLocation(activeShader, "material.diffuse"), 1, &defaultColor);
        glUniform1f(glGetUniformLocation(activeShader, "material.parallax"), 1, 0.5f);

        set_shader_int(activeShader, "diffuseMapActive", 1);
        int modelLoc = glGetUniformLocation(activeShader, "model");
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
