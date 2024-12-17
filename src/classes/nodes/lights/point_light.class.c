#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/lighting.h"
#include "buffer.h"

class PointLight @promote extends Light {
    __containerType__ Node *

    void constructor(struct PointLight *pointLight) {
        this->object = pointLight;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        PointLight *pointLight;
        pointLight = malloc(sizeof(PointLight));
        POINTER_CHECK(pointLight);

        if (file) {
            fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", 
                &pointLight->ambient[0], &pointLight->ambient[1], &pointLight->ambient[2], 
                &pointLight->diffuse[0], &pointLight->diffuse[1], &pointLight->diffuse[2], 
                &pointLight->specular[0], &pointLight->specular[1], &pointLight->specular[2],
                &pointLight->constant,
                &pointLight->linear,
                &pointLight->quadratic
                );
        } else {
            glm_vec3_zero(pointLight->ambient);
            glm_vec3_copy(GLM_VEC3_ONE, pointLight->diffuse);
            glm_vec3_copy(GLM_VEC3_ONE, pointLight->specular);
            pointLight->constant = 1.0f;
            pointLight->linear = 0.09f;
            pointLight->quadratic = 0.032f;
        }

        buffers.lightingBuffer.length++;
        METHOD_TYPE(this, __type__, constructor, pointLight);
        this->flags |= NODE_EDITOR_FLAG;
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        PointLight *pointLight = (PointLight*) this->object;
        fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
            pointLight->ambient[0], pointLight->ambient[1], pointLight->ambient[2], 
            pointLight->diffuse[0], pointLight->diffuse[1], pointLight->diffuse[2], 
            pointLight->specular[0], pointLight->specular[1], pointLight->specular[2],
            pointLight->constant,
            pointLight->linear,
            pointLight->quadratic
        );
    }

    
}
