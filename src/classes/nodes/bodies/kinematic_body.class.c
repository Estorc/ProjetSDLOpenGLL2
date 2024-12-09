#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class KinematicBody @promote extends Body {
    __containerType__ Node *

    void constructor(struct KinematicBody *kinematicBody) {
        this->object = kinematicBody;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        KinematicBody *kinematicBody;
        kinematicBody = malloc(sizeof(KinematicBody));
        kinematicBody->length = 0;
        int children_count = 0;
        POINTER_CHECK(kinematicBody);
        if (file) {
            fscanf(file,"(%g,%g,%g,%d)\n", 
                &kinematicBody->velocity[0], &kinematicBody->velocity[1], &kinematicBody->velocity[2], 
                &children_count);
        } else {
            glm_vec3_zero(kinematicBody->velocity);
        }
        METHOD_TYPE(this, CLASS_TYPE_KINEMATICBODY, constructor, kinematicBody);

        kinematicBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(kinematicBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            kinematicBody->collisionsShapes[kinematicBody->length++] = child;
            child->parent = this;
        }
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        KinematicBody *kinematicBody = (KinematicBody*) this->object;
        u8 collisionsLength = kinematicBody->length;
        fprintf(file, "(%g,%g,%g,%d)",
        kinematicBody->velocity[0], kinematicBody->velocity[1], kinematicBody->velocity[2], 
        collisionsLength);
    }


    void apply_impulse(float *impulse, float *torque, float *correction) {
        IGNORE(torque);
        KinematicBody *kinematicBody = (KinematicBody *) this->object;

        // Change velocity
        glm_vec3_add(kinematicBody->velocity, impulse, kinematicBody->velocity);

        // Move shape A out of shape B
        glm_vec3_sub(this->pos, correction, this->pos);
        for (int i = 0; i < kinematicBody->length; i++) {
            glm_vec3_sub(kinematicBody->collisionsShapes[i]->globalPos, correction, kinematicBody->collisionsShapes[i]->globalPos);
        }
    }

    
}
