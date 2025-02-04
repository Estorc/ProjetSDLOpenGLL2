#line 1 "src/__processed__/classes_nodes_cshapes_sphere_cshape.class.c"
#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_SPHERECSHAPE;
#line 10 "src/classes/nodes/cshapes/sphere_cshape.class.c"
void __class_method_spherecshape_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct SphereCollisionShape * sphereCollisionShape = va_arg(args, struct SphereCollisionShape *);(void)this;
        this->object = sphereCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/cshapes/sphere_cshape.class.c"
void __class_method_spherecshape_get_priority(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);int * priority = va_arg(args, int *);(void)this;
        *priority = 1;
}

#line 22 "src/classes/nodes/cshapes/sphere_cshape.class.c"
void __class_method_spherecshape_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        SphereCollisionShape *sphereCollisionShape;
        sphereCollisionShape = malloc(sizeof(SphereCollisionShape));
        POINTER_CHECK(sphereCollisionShape);
        this->type = __type__;
        call_method_0(METHOD(constructor,this,sphereCollisionShape));
}

#line 30 "src/classes/nodes/cshapes/sphere_cshape.class.c"
void __class_method_spherecshape_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}

    static Model _model = {0};
#line 35 "src/classes/nodes/cshapes/sphere_cshape.class.c"
void __class_method_spherecshape_get_model(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Model ** model = va_arg(args, Model **);(void)this;
        if (!_model.data) {
            load_model("assets/models/collisionShapes/sphere.obj", &_model.data);
        }
        *model = &_model;
}


    
