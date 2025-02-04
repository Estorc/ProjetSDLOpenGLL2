#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_PLANECSHAPE;
#line 10 "src/classes/nodes/cshapes/plane_cshape.class.c"
void __class_method_planecshape_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct PlaneCollisionShape * planeCollisionShape = va_arg(args, struct PlaneCollisionShape *);(void)this;
        this->object = planeCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/cshapes/plane_cshape.class.c"
void __class_method_planecshape_get_priority(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);int * priority = va_arg(args, int *);(void)this;
        *priority = 2;
}

#line 22 "src/classes/nodes/cshapes/plane_cshape.class.c"
void __class_method_planecshape_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        PlaneCollisionShape *planeCollisionShape;
        planeCollisionShape = malloc(sizeof(PlaneCollisionShape));
        POINTER_CHECK(planeCollisionShape);
        this->type = __type__;
        this::constructor(planeCollisionShape);
}

#line 30 "src/classes/nodes/cshapes/plane_cshape.class.c"
void __class_method_planecshape_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}

    static Model _model = {0};
#line 35 "src/classes/nodes/cshapes/plane_cshape.class.c"
void __class_method_planecshape_get_model(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Model ** model = va_arg(args, Model **);(void)this;
        if (!_model.data) {
            load_model("assets/models/collisionShapes/plane.obj", &_model.data);
        }
        *model = &_model;
}


    
