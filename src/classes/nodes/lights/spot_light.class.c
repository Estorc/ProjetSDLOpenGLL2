#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "render/lighting.h"
#include "buffer.h"

class SpotLight @promote extends Node {
    __containerType__ Node *

    void constructor(struct SpotLight *spotLight) {
        this->object = spotLight;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        SpotLight *spotLight;
        spotLight = malloc(sizeof(SpotLight));
        POINTER_CHECK(spotLight);

        if (file) {
            fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", 
                &spotLight->ambient[0], &spotLight->ambient[1], &spotLight->ambient[2], 
                &spotLight->diffuse[0], &spotLight->diffuse[1], &spotLight->diffuse[2], 
                &spotLight->specular[0], &spotLight->specular[1], &spotLight->specular[2],
                &spotLight->constant,
                &spotLight->linear,
                &spotLight->quadratic,
                &spotLight->cutOff,
                &spotLight->outerCutOff
                );
        } else {
            glm_vec3_zero(spotLight->ambient);
            glm_vec3_copy(GLM_VEC3_ONE, spotLight->diffuse);
            glm_vec3_copy(GLM_VEC3_ONE, spotLight->specular);
            spotLight->constant = 1.0f;
            spotLight->linear = 0.09f;
            spotLight->quadratic = 0.032f;
            spotLight->cutOff = 0.01f;
            spotLight->outerCutOff = 50.0f;
        }

        buffers.lightingBuffer.length++;
        METHOD_TYPE(this, CLASS_TYPE_SPOTLIGHT, constructor, spotLight);
        this->flags |= NODE_EDITOR_FLAG;
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        SpotLight *spotLight = (SpotLight*) this->object;
        fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
            spotLight->ambient[0], spotLight->ambient[1], spotLight->ambient[2], 
            spotLight->diffuse[0], spotLight->diffuse[1], spotLight->diffuse[2], 
            spotLight->specular[0], spotLight->specular[1], spotLight->specular[2],
            spotLight->constant,
            spotLight->linear,
            spotLight->quadratic,
            spotLight->cutOff,
            spotLight->outerCutOff
        );
    }

    
}
