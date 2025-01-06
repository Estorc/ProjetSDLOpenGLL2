#include <stdarg.h>
#include "../types.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_MESHCSHAPE;
#line 10 "src/classes/nodes/cshapes/mesh_cshape.class.c"
void __class_method_meshcshape_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct MeshCollisionShape * meshCollisionShape = va_arg(args, struct MeshCollisionShape *);(void)this;
        this->object = meshCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/cshapes/mesh_cshape.class.c"
void __class_method_meshcshape_get_priority(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);int * priority = va_arg(args, int *);(void)this;
        *priority = 4;
}

#line 22 "src/classes/nodes/cshapes/mesh_cshape.class.c"
void __class_method_meshcshape_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        Model *model;
        model = malloc(sizeof(Model));
        if (file) {
            char path[100];
            fscanf(file,"(%100[^)])", path);
            load_obj_model(path, &model->data);
        }
        MeshCollisionShape *meshCollisionShape;
        meshCollisionShape = malloc(sizeof(MeshCollisionShape));
        meshCollisionShape->facesVertex = model->data->objects[0].facesVertex;
        meshCollisionShape->numFaces = model->data->objects[0].length;
        POINTER_CHECK(meshCollisionShape);
        this->type = __type__;
        this::constructor(meshCollisionShape);
}

#line 39 "src/classes/nodes/cshapes/mesh_cshape.class.c"
void __class_method_meshcshape_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}




    
