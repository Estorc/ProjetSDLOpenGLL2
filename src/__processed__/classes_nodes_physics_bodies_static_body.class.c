#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../io/scene_loader.h"
#include "../render/lighting.h"
#include "../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_STATICBODY;
#line 13 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct StaticBody * staticBody = va_arg(args, struct StaticBody *);(void)this;
        this->object = staticBody;
        this->type = __type__;
        SUPER(initialize_node);
}

#line 19 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_get_collisions_shapes(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node **** shapes = va_arg(args, Node ****);u8 ** length = va_arg(args, u8 **);(void)this;
        StaticBody *staticBody = (StaticBody *) this->object;
        *length = &staticBody->length;
        *shapes = &staticBody->collisionsShapes;
}

#line 25 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * pos = va_arg(args, vec3 *);vec3 * rot = va_arg(args, vec3 *);vec3 * scale = va_arg(args, vec3 *);(void)this;
        StaticBody *staticBody = (StaticBody *) this->object;

        this::update_global_position(pos, rot, scale);

        for (int i = 0; i < staticBody->length; i++) {
            (staticBody->collisionsShapes[i])::update_global_position(pos, rot, scale);
            glm_vec3_copy(this->globalPos, *pos);
            glm_vec3_copy(this->globalRot, *rot);
            glm_vec3_copy(this->globalScale, *scale);
            check_collisions(staticBody->collisionsShapes[i]);
        }
        memcpy(&buffers.collisionBuffer.collisionsShapes[buffers.collisionBuffer.index], staticBody->collisionsShapes, staticBody->length * sizeof(staticBody->collisionsShapes[0]));
        buffers.collisionBuffer.index += staticBody->length;
}

#line 41 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Camera ** c = va_arg(args, Camera **);Script * scripts = va_arg(args, Script *);Node * editor = va_arg(args, Node *);(void)this;
        StaticBody *staticBody;
        staticBody = malloc(sizeof(StaticBody));
        staticBody->length = 0;
        int children_count = 0;
        POINTER_CHECK(staticBody);
        if (file) {
            fscanf(file,"(%d", &children_count);
            if (fgetc(file) != ')') {
                fscanf(file, "%f,%f,%f)\n", &staticBody->forcedNormal[0], &staticBody->forcedNormal[1], &staticBody->forcedNormal[2]);
            } else {
                fscanf(file, "\n");
                memset(staticBody->forcedNormal, 0, sizeof(staticBody->forcedNormal));
            }
        }
        this->type = __type__;
        this::constructor(staticBody);

        staticBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(staticBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            staticBody->collisionsShapes[staticBody->length++] = child;
            child->parent = this;
        }
}

#line 70 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
        StaticBody *staticBody = (StaticBody*) this->object;
        u8 collisionsLength = staticBody->length;
        fprintf(file, "(%d)", collisionsLength);
}

    /**
     * @brief Get the velocity norm of a node.
     * 
     * @return The velocity norm of the node.
     */

#line 83 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_get_velocity_norm(void * __retValueVP__, va_list args) {float * __retValueP__ = (float *) __retValueVP__;Node * this = va_arg(args, Node *);(void)this;
        *__retValueP__ = 0.0f;return;
}

    /**
     * @brief Get the velocity of a node.
     * 
     * @param velocity Output vector to store the velocity.
     */

#line 93 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_get_velocity(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * velocity = va_arg(args, vec3 *);(void)this;
        glm_vec3_zero(*velocity);
}

    /**
     * @brief Get the mass of a node.
     * 
     * @param mass Output pointer to store the mass.
     */

#line 103 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_get_mass(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float *  mass = va_arg(args, float * );(void)this;
        (*mass) = INFINITY;
}

    /**
     * @brief Get the center of mass of a node.
     * 
     * @param com Output vector to store the center of mass.
     */

#line 113 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_get_center_of_mass(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * com = va_arg(args, vec3 *);(void)this;
        glm_vec3_zero(*com);
}



#line 119 "src/classes/nodes/physics/bodies/static_body.class.c"
void __class_method_staticbody_get_collision_normal(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float * normal = va_arg(args, float *);(void)this;
        StaticBody *staticBody = (StaticBody *) this->object;
        glm_vec3_copy(staticBody->forcedNormal, normal);
}

