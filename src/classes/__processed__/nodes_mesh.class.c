#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../node.h"
void __class_method_mesh_constructor(unsigned type, ...) {
unsigned __type__ = 15;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct Mesh * mesh = va_arg(args, struct Mesh *);
va_end(args);
(void)this;
    this->object = mesh;
    this->type = __type__;
    SUPER(initialize_node);
}

void __class_method_mesh_cast(unsigned type, ...) {
unsigned __type__ = 15;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}

void __class_method_mesh_load(unsigned type, ...) {
unsigned __type__ = 15;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    Mesh *mesh;
    mesh = malloc(sizeof(Mesh));
    POINTER_CHECK(mesh);
    create_screen_plane(mesh);
    METHOD_TYPE(this, CLASS_TYPE_MESH, constructor, mesh);
}

void __class_method_mesh_save(unsigned type, ...) {
unsigned __type__ = 15;
(void)__type__;
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}

void __class_method_mesh_render(unsigned type, ...) {
unsigned __type__ = 15;
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
    Mesh *mesh = (Mesh *)this->object;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(mesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh->length);
    glBindVertexArray(0);
}

