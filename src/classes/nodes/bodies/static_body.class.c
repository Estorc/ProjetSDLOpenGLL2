#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class StaticBody @promote extends Body {
    __containerType__ Node *

    void constructor(struct StaticBody *staticBody) {
        this->object = staticBody;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        StaticBody *staticBody;
        staticBody = malloc(sizeof(StaticBody));
        staticBody->length = 0;
        int children_count = 0;
        POINTER_CHECK(staticBody);
        if (file)
            fscanf(file,"(%d)\n", &children_count);
        METHOD_TYPE(this, __type__, constructor, staticBody);

        staticBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(staticBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            staticBody->collisionsShapes[staticBody->length++] = child;
            child->parent = this;
        }
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        StaticBody *staticBody = (StaticBody*) this->object;
        u8 collisionsLength = staticBody->length;
        fprintf(file, "(%d)", collisionsLength);
    }
}
