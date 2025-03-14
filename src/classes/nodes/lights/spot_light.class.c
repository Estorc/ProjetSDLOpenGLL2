/**
 * @file
 * @brief This file contains the implementation of the SpotLight class.
 *
 * This source file contains the implementation details for the SpotLight class,
 * which is used to manage and manipulate spotlights in an OpenGL context. The
 * SpotLight class provides functionalities to set up, configure, and control
 * spotlights, including their position, direction, and lighting properties.
 *
 * The code is designed to integrate with an SDL and OpenGL based rendering
 * engine, allowing for dynamic lighting effects in 3D scenes.
 *
 * @author Loup Picault
 * @date 2023-10-20
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/lighting.h"
#include "render/depth_map.h"
#include "buffer.h"

class SpotLight : public Light {
    __containerType__ Node *
    public:

    void constructor(struct SpotLight *spotLight) {
        this->object = spotLight;
        this->type = __type__;
        SUPER(initialize_node);
        SUPER(init_light);
    }

    void update(vec3 *pos, vec3 *rot, vec3 *scale, double delta, u8 *lightsCount) {
        UNUSED(delta);
        if (!(this->flags & NODE_LIGHT_ACTIVE)) return;
        SpotLight *spotLight = (SpotLight *) this->object;

        this::update_global_position(pos, rot, scale);

        const char uniforms[11][20] = {
            "].position",
            "].direction",
            "].color",
            "].bias",
            "].size",
            "].constant",
            "].linear",
            "].quadratic",
            "].cutOff",
            "].outerCutOff",
            "].index"
        };

        char uniformsName[11][50];

        for (int i = 0; i < 11; i++) {
            strcpy(uniformsName[i], "spotLights[");
            sprintf(uniformsName[i] + strlen(uniformsName[i]), "%d", lightsCount[SPOT_LIGHT]);
            strcat(uniformsName[i], uniforms[i]);
        }

        vec3 dir = {0.0, 0.0, -1.0};
        glm_vec3_rotate(dir, to_radians(this->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_vec3_rotate(dir, to_radians(this->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_vec3_rotate(dir, to_radians(this->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

        for (int i = 0; i < Game.memoryCaches->shadersCount; i++) {
            use_shader(Game.memoryCaches->shaderCache[i].shader);
            set_shader_vec3(Game.memoryCaches->shaderCache[i].shader, uniformsName[0], this->globalPos);
            set_shader_vec3(Game.memoryCaches->shaderCache[i].shader, uniformsName[1], dir);
            set_shader_vec3(Game.memoryCaches->shaderCache[i].shader, uniformsName[2], spotLight->color);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[3], spotLight->bias);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[4], spotLight->size);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[5], spotLight->constant);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[6], spotLight->linear);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[7], spotLight->quadratic);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[8], spotLight->cutOff);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[9], spotLight->outerCutOff);
            set_shader_int(Game.memoryCaches->shaderCache[i].shader, uniformsName[10], lightsCount[DIRECTIONAL_LIGHT] + lightsCount[POINT_LIGHT]*6 + lightsCount[SPOT_LIGHT]);
        }

        Game.buffers->lightingBuffer.lightings[Game.buffers->lightingBuffer.index++] = this;
        lightsCount[SPOT_LIGHT]++;
    }   



    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        SpotLight *spotLight = (SpotLight *) this->object;
        void *data[] = {
            "rgb", "Color : ", &spotLight->color,
            "float", "Bias : ", &spotLight->bias,
            "float", "Size : ", &spotLight->size,
            "float", "Constant : ", &spotLight->constant,
            "float", "Linear : ", &spotLight->linear,
            "float", "Quadratic : ", &spotLight->quadratic,
            "float", "CutOff : ", &spotLight->cutOff,
            "float", "OuterCutOff : ", &spotLight->outerCutOff
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    void load(FILE *file) {
        SpotLight *spotLight;
        spotLight = malloc(sizeof(SpotLight));
        POINTER_CHECK(spotLight);

        if (file) {
            fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", 
                &spotLight->color[0], &spotLight->color[1], &spotLight->color[2], 
                &spotLight->bias,
                &spotLight->size,
                &spotLight->constant,
                &spotLight->linear,
                &spotLight->quadratic,
                &spotLight->cutOff,
                &spotLight->outerCutOff
                );
        } else {
            glm_vec3_one(spotLight->color);
            spotLight->bias = 0.005f;
            spotLight->size = 0.0f;
            spotLight->constant = 1.0f;
            spotLight->linear = 0.09f;
            spotLight->quadratic = 0.032f;
            spotLight->cutOff = 0.01f;
            spotLight->outerCutOff = 50.0f;
        }

        Game.buffers->lightingBuffer.length++;
        this->type = __type__;
        this::constructor(spotLight);
        this->flags |= NODE_EDITOR_FLAG;
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        SpotLight *spotLight = (SpotLight*) this->object;
        fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
            spotLight->color[0], spotLight->color[1], spotLight->color[2], 
            spotLight->bias,
            spotLight->size,
            spotLight->constant,
            spotLight->linear,
            spotLight->quadratic,
            spotLight->cutOff,
            spotLight->outerCutOff
        );
    }


    /**
     * @brief Configures the lighting.
     * 
     * This function configures the lighting.
     * 
     * @param c The camera to be used for rendering.
     * @param shaders The shaders to be used for rendering.
     * @param lightsCount The number of lights in the scene.
     */

    void configure_lighting(Camera *c, WorldShaders *shaders, DepthMap *depthMap, u8 *lightsCount) {

        UNUSED(c);
        // Lights and shadows
        mat4 lightProjection;
        mat4 lightView = GLM_MAT4_IDENTITY_INIT;

        size_t storageBufferIndex;

        f32 near_plane = 1.0f, far_plane = 200.0f;
        glm_perspective(to_radians(90.0f), 1.0f, near_plane, far_plane, lightProjection);

        vec3 rot, pos;
        glm_vec3_negate_to(this->globalPos, pos);
        glm_vec3_negate_to(this->globalRot, rot);

        glm_rotate(lightView, to_radians(rot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(lightView, to_radians(rot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(lightView, to_radians(rot[2]), (vec3){0.0f, 0.0f, 1.0f});
        glm_translate(lightView, (vec3){pos[0], pos[1], pos[2]});

        storageBufferIndex = lightsCount[SPOT_LIGHT]+(NUM_DIRECTIONAL_LIGHTS+NUM_POINT_LIGHTS*6);
        lightsCount[SPOT_LIGHT]++;


        SUPER(configure_lighting, shaders, lightView, lightProjection, storageBufferIndex, depthMap);
    }

    
}
