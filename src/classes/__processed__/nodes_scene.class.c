#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../storage/node.h"
#include "../../io/scene_loader.h"
#include "../../render/framebuffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_SCENE;


void __class_method_scene_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_scene_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_scene_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Camera ** c = va_arg(args, Camera **);
Script * scripts = va_arg(args, Script *);
va_end(args);
(void)this;
    METHOD_TYPE(this, __type__, constructor);
    char path[256];
    if (file) {
        fscanf(file,"(%s)\n", 
            path);
    }
    this->children = realloc(this->children, sizeof(Node *));
    add_child(this, load_scene(path, c, scripts));
}


void __class_method_scene_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}




void __class_method_scene_render(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
mat4 * modelMatrix = va_arg(args, mat4 *);
Shader  activeShader = va_arg(args, Shader );
va_end(args);
(void)this;
    int modelLoc = glGetUniformLocation(activeShader, "model");
    Mesh *mesh = (Mesh *)this->object;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(mesh->VAO);

    glDrawArrays(GL_TRIANGLES, 0, mesh->length);
    glBindVertexArray(0);
}


    

