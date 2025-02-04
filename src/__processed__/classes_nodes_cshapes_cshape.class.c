#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CSHAPE;
#line 10 "src/classes/nodes/cshapes/cshape.class.c"
void __class_method_cshape_is_cshape(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * cshape = va_arg(args, bool *);(void)this;
        (*cshape) = true;
}

#line 14 "src/classes/nodes/cshapes/cshape.class.c"
void __class_method_cshape_get_model(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Model ** model = va_arg(args, Model **);(void)this;
        *model = NULL;
}
    
    static Shader collisionShader;
#line 19 "src/classes/nodes/cshapes/cshape.class.c"
void __class_method_cshape_get_shader(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Shader * shader = va_arg(args, Shader *);(void)this;
        if (!collisionShader) {
            collisionShader = create_shader("shaders/collision_debug.vs", "shaders/collision_debug.fs");
        }
        *shader = collisionShader;
}


#line 27 "src/classes/nodes/cshapes/cshape.class.c"
void __class_method_cshape_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );(void)this;
        Model *model;
        this::get_model(&model);
        Shader shader;
        this::get_shader(&shader);
        use_shader(shader);
        if (model) render_model(modelMatrix, shader, model);
}

