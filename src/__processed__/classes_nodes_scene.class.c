#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../storage/node.h"
#include "../io/scene_loader.h"
#include "../render/framebuffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_SCENE;
#line 11 "src/classes/nodes/scene.class.c"
void __class_method_scene_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/scene.class.c"
void __class_method_scene_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Camera ** c = va_arg(args, Camera **);Script * scripts = va_arg(args, Script *);(void)this;
        this->type = __type__;
        this::constructor();
        char path[256];
        if (file) {
            fscanf(file,"(%s)\n", 
                path);
        }
        this->children = realloc(this->children, sizeof(Node *));
        this::add_child(load_scene(path, c, scripts));
}

#line 30 "src/classes/nodes/scene.class.c"
void __class_method_scene_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}


#line 35 "src/classes/nodes/scene.class.c"
void __class_method_scene_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );(void)this;
        int modelLoc = glGetUniformLocation(activeShader, "model");
        Mesh *mesh = (Mesh *)this->object;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

        glBindVertexArray(mesh->VAO);

        glDrawArrays(GL_TRIANGLES, 0, mesh->length);
        glBindVertexArray(0);
}

    
