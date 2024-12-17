#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/lighting.h"
#include "buffer.h"

class DirectionalLight @promote extends Light {
    __containerType__ Node *

    void constructor(struct DirectionalLight *directionalLight) {
        this->object = directionalLight;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        DirectionalLight *directionalLight;
        directionalLight = malloc(sizeof(DirectionalLight));
        POINTER_CHECK(directionalLight);

        if (file) {
            fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", 
                &directionalLight->ambient[0], &directionalLight->ambient[1], &directionalLight->ambient[2], 
                &directionalLight->diffuse[0], &directionalLight->diffuse[1], &directionalLight->diffuse[2], 
                &directionalLight->specular[0], &directionalLight->specular[1], &directionalLight->specular[2],
                &directionalLight->constant,
                &directionalLight->linear,
                &directionalLight->quadratic
                );
        } else {
            glm_vec3_zero(directionalLight->ambient);
            glm_vec3_copy(GLM_VEC3_ONE, directionalLight->diffuse);
            glm_vec3_copy(GLM_VEC3_ONE, directionalLight->specular);
            directionalLight->constant = 1.0f;
            directionalLight->linear = 0.09f;
            directionalLight->quadratic = 0.032f;
        }

        buffers.lightingBuffer.length++;
        METHOD_TYPE(this, __type__, constructor, directionalLight);
        this->flags |= NODE_EDITOR_FLAG;
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        DirectionalLight *directionalLight = (DirectionalLight*) this->object;
        fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
            directionalLight->ambient[0], directionalLight->ambient[1], directionalLight->ambient[2], 
            directionalLight->diffuse[0], directionalLight->diffuse[1], directionalLight->diffuse[2], 
            directionalLight->specular[0], directionalLight->specular[1], directionalLight->specular[2],
            directionalLight->constant,
            directionalLight->linear,
            directionalLight->quadratic
        );
    }


    
}
