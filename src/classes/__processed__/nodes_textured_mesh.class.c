#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
#include "../../memory.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_TEXTUREDMESH;


void __class_method_texturedmesh_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct TexturedMesh * texturedMesh = va_arg(args, struct TexturedMesh *);
va_end(args);
(void)this;
    this->object = texturedMesh;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_texturedmesh_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_texturedmesh_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
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
    METHOD_TYPE(this, __type__, constructor, texturedMesh);
}


void __class_method_texturedmesh_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    TextureMap texture = ((TexturedMesh*) this->object)->texture;
    for (int i = 0; i < memoryCaches.texturesCount; i++) {
        if (memoryCaches.textureCache[i].textureMap == texture) {
            fprintf(file, "(%s)", memoryCaches.textureCache[i].textureName);
            break;
        }
    }
}




void __class_method_texturedmesh_render(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
mat4 * modelMatrix = va_arg(args, mat4 *);
Shader  activeShader = va_arg(args, Shader );
va_end(args);
(void)this;
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


    

