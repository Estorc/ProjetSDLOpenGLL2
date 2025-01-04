#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "memory.h"
#include "render/render.h"

class Skybox : public Node {
    __containerType__ Node *
    public:

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
        char path[6][100];
        if (file) {
            fscanf(file,"(%100[^,],%100[^,],%100[^,],%100[^,],%100[^,],%100[^)])", 
            path[0],
            path[1],
            path[2],
            path[3],
            path[4],
            path[5]
            );
        } else {
            path[0][0] = path[1][0] = path[2][0] = path[3][0] = path[4][0] = path[5][0] = 0;
        }
        create_skybox(texturedMesh, path);
        METHOD_TYPE(this, __type__, constructor, texturedMesh);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        TextureMap texture = ((TexturedMesh*) this->object)->texture;
        for (int i = 0; i < memoryCaches.cubeMapCount; i++) {
            if (memoryCaches.cubeMapCache[i].cubeMap == texture) {
                fprintf(file, "(%s,%s,%s,%s,%s,%s)",
                    memoryCaches.cubeMapCache[i].textureName[0],
                    memoryCaches.cubeMapCache[i].textureName[1],
                    memoryCaches.cubeMapCache[i].textureName[2],
                    memoryCaches.cubeMapCache[i].textureName[3],
                    memoryCaches.cubeMapCache[i].textureName[4],
                    memoryCaches.cubeMapCache[i].textureName[5]
                );
                break;
            }
        }
    }


    void render(mat4 *modelMatrix, Shader activeShader, WorldShaders *shaders) {
        IGNORE(activeShader);
        
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

        Shader shader = shaders->skybox;
        use_shader(shader);
        int modelLoc = glGetUniformLocation(shader, "model");
        TexturedMesh *texturedMesh = (TexturedMesh *)this->object;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        // render Cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texturedMesh->texture);
        set_shader_int(shader, "skybox", 0);
        glBindVertexArray(texturedMesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, texturedMesh->length);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS); // set depth function back to default
    }

    
}
