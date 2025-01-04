#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
#include "../../memory.h"
#include "../../render/render.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_SKYBOX;

void __class_method_skybox_constructor(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
struct TexturedMesh * texturedMesh = va_arg(args, struct TexturedMesh *);
(void)this;
    this->object = texturedMesh;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_skybox_cast(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
(void)this;
    IGNORE(data);
}

void __class_method_skybox_load(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
(void)this;
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

void __class_method_skybox_save(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
(void)this;
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


void __class_method_skybox_render(void * __retValueVP__, va_list args) {
Node * this = va_arg(args, Node *);
mat4 * modelMatrix = va_arg(args, mat4 *);
Shader  activeShader = va_arg(args, Shader );
WorldShaders * shaders = va_arg(args, WorldShaders *);
(void)this;
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

    
