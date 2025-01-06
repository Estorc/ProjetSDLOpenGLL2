#line 1 "src/__processed__/classes_nodes_cshapes_ray_cshape.class.c"
#include <stdarg.h>
#include "../types.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_RAYCSHAPE;
#line 10 "src/classes/nodes/cshapes/ray_cshape.class.c"
void __class_method_raycshape_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct RayCollisionShape * rayCollisionShape = va_arg(args, struct RayCollisionShape *);(void)this;
        this->object = rayCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/cshapes/ray_cshape.class.c"
void __class_method_raycshape_get_priority(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);int * priority = va_arg(args, int *);(void)this;
        *priority = 5;
}

#line 22 "src/classes/nodes/cshapes/ray_cshape.class.c"
void __class_method_raycshape_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        RayCollisionShape *rayCollisionShape;
        rayCollisionShape = malloc(sizeof(RayCollisionShape));
        POINTER_CHECK(rayCollisionShape);
        this->type = __type__;
        call_method_0(METHOD(constructor,this,rayCollisionShape));
}

#line 30 "src/classes/nodes/cshapes/ray_cshape.class.c"
void __class_method_raycshape_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}




    
