#line 1 "src/__processed__/classes_nodes_textured_mesh.class.c"
#include <stdarg.h>
#include "../types.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../memory.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_TEXTUREDMESH;
#line 11 "src/classes/nodes/textured_mesh.class.c"
void __class_method_texturedmesh_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct TexturedMesh * texturedMesh = va_arg(args, struct TexturedMesh *);(void)this;
        this->object = texturedMesh;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 19 "src/classes/nodes/textured_mesh.class.c"
void __class_method_texturedmesh_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
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
        call_method_0(METHOD(constructor,this,texturedMesh));
}

#line 34 "src/classes/nodes/textured_mesh.class.c"
void __class_method_texturedmesh_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
        TextureMap texture = ((TexturedMesh*) this->object)->texture;
        for (int i = 0; i < memoryCaches.texturesCount; i++) {
            if (memoryCaches.textureCache[i].textureMap == texture) {
                fprintf(file, "(%s)", memoryCaches.textureCache[i].textureName);
                break;
            }
        }
}


#line 46 "src/classes/nodes/textured_mesh.class.c"
void __class_method_texturedmesh_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );(void)this;
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

    
