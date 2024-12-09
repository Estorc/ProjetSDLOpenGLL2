#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class RigidBody @promote extends Body {
    __containerType__ Node *

    void constructor(struct RigidBody *rigidBody) {
        this->object = rigidBody;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        RigidBody *rigidBody;
        rigidBody = malloc(sizeof(RigidBody));
        rigidBody->length = 0;
        int children_count = 0;
        POINTER_CHECK(rigidBody);
        if (file) {
            fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%d)\n", 
                &rigidBody->velocity[0], &rigidBody->velocity[1], &rigidBody->velocity[2], 
                &rigidBody->angularVelocity[0], &rigidBody->angularVelocity[1], &rigidBody->angularVelocity[2], 
                &rigidBody->gravity[0], &rigidBody->gravity[1], &rigidBody->gravity[2],
                &rigidBody->mass,
                &rigidBody->friction,
                &rigidBody->centerOfMass[0], &rigidBody->centerOfMass[1], &rigidBody->centerOfMass[2],
                &children_count);
        } else {
            glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->velocity);
            glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->angularVelocity);
            glm_vec3_copy((vec3) {0.0,-1.0,0.0}, rigidBody->gravity);
            rigidBody->mass = 100.0;
            rigidBody->friction = 0.98;
            glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->centerOfMass);
        }
        METHOD_TYPE(this, CLASS_TYPE_RIGIDBODY, constructor, rigidBody);

        rigidBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(rigidBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            rigidBody->collisionsShapes[rigidBody->length++] = child;
            child->parent = this;
        }
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        RigidBody *rigidBody = (RigidBody*) this->object;
        u8 collisionsLength = rigidBody->length;
        fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%d)",
        rigidBody->velocity[0], rigidBody->velocity[1], rigidBody->velocity[2], 
        rigidBody->angularVelocity[0], rigidBody->angularVelocity[1], rigidBody->angularVelocity[2], 
        rigidBody->gravity[0], rigidBody->gravity[1], rigidBody->gravity[2],
        rigidBody->mass,
        rigidBody->friction,
        rigidBody->centerOfMass[0], rigidBody->centerOfMass[1], rigidBody->centerOfMass[2],
        collisionsLength);
    }

    void apply_impulse(float *impulse, float *torque, float *correction, float *momentOfInertia) {
        RigidBody *rigidBody = (RigidBody *) this->object;

        // Change velocity
        glm_vec3_add(rigidBody->velocity, impulse, rigidBody->velocity);

        // Move shape A out of shape B
        glm_vec3_sub(this->pos, correction, this->pos);
        for (int i = 0; i < rigidBody->length; i++) {
            glm_vec3_sub(rigidBody->collisionsShapes[i]->globalPos, correction, rigidBody->collisionsShapes[i]->globalPos);
        }

        // Apply torque
        glm_vec3_scale(torque, *momentOfInertia*50.0f, torque);
        glm_vec3_add(rigidBody->angularVelocity, torque, rigidBody->angularVelocity);
    }


    
}
