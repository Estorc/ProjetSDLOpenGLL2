#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class MeshCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor(struct MeshCollisionShape *meshCollisionShape) {
        this->object = meshCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void get_priority(int *priority) {
        *priority = 4;
    }

    void load(FILE *file) {
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
        METHOD_TYPE(this, __type__, constructor, meshCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
