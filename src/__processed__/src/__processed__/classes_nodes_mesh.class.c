#line 1 "src/__processed__/classes_nodes_mesh.class.c"
#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_MESH;
#line 10 "src/classes/nodes/mesh.class.c"
void __class_method_mesh_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct Mesh * mesh = va_arg(args, struct Mesh *);(void)this;
        this->object = mesh;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/mesh.class.c"
void __class_method_mesh_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Mesh *mesh;
        mesh = malloc(sizeof(Mesh));
        POINTER_CHECK(mesh);
        create_screen_plane(mesh);
        this->type = __type__;
        call_method_0(METHOD(constructor,this,mesh));
}

#line 27 "src/classes/nodes/mesh.class.c"
void __class_method_mesh_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}


#line 32 "src/classes/nodes/mesh.class.c"
void __class_method_mesh_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );(void)this;
        int modelLoc = glGetUniformLocation(activeShader, "model");
        Mesh *mesh = (Mesh *)this->object;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh->length);
        glBindVertexArray(0);
}

    
