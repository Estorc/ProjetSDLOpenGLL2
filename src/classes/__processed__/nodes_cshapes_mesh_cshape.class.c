#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_MESHCSHAPE;


void __class_method_meshcshape_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct MeshCollisionShape * meshCollisionShape = va_arg(args, struct MeshCollisionShape *);
va_end(args);
(void)this;
    this->object = meshCollisionShape;
    this->type = __type__;
    SUPER(initialize_node);
}


void __class_method_meshcshape_cast(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
void **  data = va_arg(args, void ** );
va_end(args);
(void)this;
    IGNORE(data);
}


void __class_method_meshcshape_get_priority(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
int * priority = va_arg(args, int *);
va_end(args);
(void)this;
    *priority = 4;
}


void __class_method_meshcshape_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    Model *model;
    if (file) {
        char path[100];
        fscanf(file,"(%100[^)])", path);
        load_obj_model(path, &model);
    }
    MeshCollisionShape *meshCollisionShape;
    meshCollisionShape = malloc(sizeof(MeshCollisionShape));
    meshCollisionShape->facesVertex = model->objects[0].facesVertex;
    meshCollisionShape->numFaces = model->objects[0].length;
    POINTER_CHECK(meshCollisionShape);
    METHOD_TYPE(this, __type__, constructor, meshCollisionShape);
}


void __class_method_meshcshape_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
}








    

