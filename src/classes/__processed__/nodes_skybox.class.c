#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
#include "../../memory.h"
void __class_method_skybox_constructor(unsigned type, ...) {
unsigned __type__ = 17;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct TexturedMesh * texturedMesh = va_arg(args, struct TexturedMesh *);
va_end(args);
(void)this;
    this->object = texturedMesh;
    this->type = __type__;
    SUPER(initialize_node);
    this->shader = create_shader(DEFAULT_SKYBOX_SHADER);
}

void __class_method_skybox_cast(unsigned type, ...) {
unsigned __type__ = 17;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_skybox_load(unsigned type, ...) {
unsigned __type__ = 17;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
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
    METHOD_TYPE(this, CLASS_TYPE_SKYBOX, constructor, texturedMesh);
}

void __class_method_skybox_save(unsigned type, ...) {
unsigned __type__ = 17;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
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

void __class_method_skybox_render(unsigned type, ...) {
unsigned __type__ = 17;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
mat4 * modelMatrix = va_arg(args, mat4 *);
va_end(args);
(void)this;
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    Shader shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, (int*) &shader);

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

