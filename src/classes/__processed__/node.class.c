#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_NODE;
#line 10 "src/classes/node.class.c"
void __class_method_node_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
    this->object = NULL;
    this->type = __type__;
    METHOD(this, initialize_node);
}

#line 16 "src/classes/node.class.c"
void __class_method_node_initialize_node(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
    this->flags = NODE_DEFAULT_FLAGS;
    this->length = 0;
    this->children = NULL;
    this->behavior = NULL;
    this->params = NULL;
    this->params_count = 0;
    this->shader = 0;
    glm_vec3_zero(this->pos);
    glm_vec3_zero(this->rot);
    glm_vec3_one(this->scale);
}

    static Shader glowShader = 0;
#line 30 "src/classes/node.class.c"
void __class_method_node_get_glow_shader(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Shader * shader = va_arg(args, Shader *);(void)this;
    if (!glowShader) {
        glowShader = create_shader("shaders/glow.vs", "shaders/glow.fs");
    }
    *shader = glowShader;
}

#line 37 "src/classes/node.class.c"
void __class_method_node_cast(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);void **  data = va_arg(args, void ** );(void)this;
    IGNORE(data);
}

#line 41 "src/classes/node.class.c"
void __class_method_node_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
    METHOD_TYPE(this, __type__, constructor);
}

#line 45 "src/classes/node.class.c"
void __class_method_node_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}

#line 49 "src/classes/node.class.c"
void __class_method_node_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
    //
}

#line 53 "src/classes/node.class.c"
void __class_method_node_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
    //
}

#line 57 "src/classes/node.class.c"
void __class_method_node_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
    for (int i = 0; i < this->length; i++) {
        METHOD(this->children[i], free);
    }
    free(this->object);
    free(this->params);
    free(this->children);
    free(this);
}

#line 67 "src/classes/node.class.c"
void __class_method_node_is_cshape(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * cshape = va_arg(args, bool *);(void)this;
    *cshape = false;
}

#line 71 "src/classes/node.class.c"
void __class_method_node_is_body(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * body = va_arg(args, bool *);(void)this;
    *body = false;
}

#line 75 "src/classes/node.class.c"
void __class_method_node_is_area(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * area = va_arg(args, bool *);(void)this;
    *area = false;
}

#line 79 "src/classes/node.class.c"
void __class_method_node_is_gui_element(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
    *result = false;
}

#line 83 "src/classes/node.class.c"
void __class_method_node_test_function(void * __retValueVP__, va_list args) {int * __retValueP__ = (int *) __retValueVP__;Node * this = va_arg(args, Node *);int  a = va_arg(args, int );int  b = va_arg(args, int );(void)this;
    printf("a: %d, b: %d\n", a, b);
    *__retValueP__ = a + b;return;
}
