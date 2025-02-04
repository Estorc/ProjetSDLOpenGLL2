#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_BOXCSHAPE;
#line 10 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct BoxCollisionShape * boxCollisionShape = va_arg(args, struct BoxCollisionShape *);(void)this;
        this->object = boxCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_get_priority(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);int * priority = va_arg(args, int *);(void)this;
        *priority = 0;
}

#line 22 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        BoxCollisionShape *boxCollisionShape;
        boxCollisionShape = malloc(sizeof(BoxCollisionShape));
        POINTER_CHECK(boxCollisionShape);
        this->type = __type__;
        this::constructor(boxCollisionShape);
}

#line 30 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}








    static Model _model = {0};
#line 42 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_get_model(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Model ** model = va_arg(args, Model **);(void)this;
        if (!_model.data) {
            load_model("assets/models/collisionShapes/box.obj", &_model.data);
        }
        *model = &_model;
}

    
