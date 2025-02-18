/**
 * @file
 * @brief This file contains the implementation of the directional light class.
 *
 * This source file contains the implementation details for the directional light class used in the project.
 * It is designed to handle the properties and behaviors of directional lights in the rendering engine.
 * The directional light class is responsible for setting up light direction, intensity, and other related parameters
 * to simulate directional lighting in the scene.
 *
 * @author Loup Picault
 * @date 2023-10-31
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

class DirectionalLight : public Light {
    __containerType__ Node *
    public:

    void constructor(struct DirectionalLight *directionalLight) {
        this->object = directionalLight;
        this->type = __type__;
        SUPER(initialize_node);
        SUPER(init_light);
    }

    void update(vec3 *pos, vec3 *rot, vec3 *scale, double delta, u8 *lightsCount) {
        UNUSED(delta);
        if (!(this->flags & NODE_LIGHT_ACTIVE)) return;
        DirectionalLight *directionalLight = (DirectionalLight *) this->object;

        this::update_global_position(pos, rot, scale);

        const char uniforms[8][20] = {
            "].position",
            "].direction",
            "].color",
            "].bias",
            "].constant",
            "].linear",
            "].quadratic",
            "].index"
        };

        char uniformsName[8][50];

        for (int i = 0; i < 8; i++) {
            strcpy(uniformsName[i], "dirLights[");
            sprintf(uniformsName[i] + strlen(uniformsName[i]), "%d", lightsCount[DIRECTIONAL_LIGHT]);
            strcat(uniformsName[i], uniforms[i]);
        }

        vec3 dir = {1.0, 0.0, 0.0};

        glm_vec3_rotate(dir, to_radians(this->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_vec3_rotate(dir, to_radians(this->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_vec3_rotate(dir, to_radians(this->rot[2]), (vec3){0.0f, 0.0f, 1.0f});

        for (int i = 0; i < memoryCaches.shadersCount; i++) {
            use_shader(memoryCaches.shaderCache[i].shader);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[0], this->globalPos);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[1], dir);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[2], directionalLight->color);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[3], directionalLight->bias);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[4], directionalLight->constant);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[5], directionalLight->linear);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[6], directionalLight->quadratic);
            set_shader_int(memoryCaches.shaderCache[i].shader, uniformsName[7], lightsCount[DIRECTIONAL_LIGHT] + lightsCount[POINT_LIGHT]*6 + lightsCount[SPOT_LIGHT]);
        }

        buffers.lightingBuffer.lightings[buffers.lightingBuffer.index++] = this;
        lightsCount[DIRECTIONAL_LIGHT]++;
    }   



    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        DirectionalLight *directionalLight = (DirectionalLight*) this->object;
        void *data[] = {
            "rgb", "Color : ", &directionalLight->color,
            "float", "Bias : ", &directionalLight->bias,
            "float", "Constant : ", &directionalLight->constant,
            "float", "Linear : ", &directionalLight->linear,
            "float", "Quadratic : ", &directionalLight->quadratic
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    void load(FILE *file) {
        DirectionalLight *directionalLight;
        directionalLight = malloc(sizeof(DirectionalLight));
        POINTER_CHECK(directionalLight);

        if (file) {
            fscanf(file,"(%g,%g,%g,%g,%g,%g,%g)\n", 
                &directionalLight->color[0], &directionalLight->color[1], &directionalLight->color[2], 
                &directionalLight->bias,
                &directionalLight->constant,
                &directionalLight->linear,
                &directionalLight->quadratic
                );
        } else {
            glm_vec3_one(directionalLight->color);
            directionalLight->bias = 0.005f;
            directionalLight->constant = 1.0f;
            directionalLight->linear = 0.09f;
            directionalLight->quadratic = 0.032f;
        }

        buffers.lightingBuffer.length++;
        this->type = __type__;
        this::constructor(directionalLight);
        this->flags |= NODE_EDITOR_FLAG;
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        DirectionalLight *directionalLight = (DirectionalLight*) this->object;
        fprintf(file, "(%g,%g,%g,%g,%g,%g,%g)",
            directionalLight->color[0], directionalLight->color[1], directionalLight->color[2], 
            directionalLight->bias,
            directionalLight->constant,
            directionalLight->linear,
            directionalLight->quadratic
        );
    }

    /**
     * @brief Configures the directional lighting for the specified camera.
     * 
     * This function configures the directional lighting for the specified camera.
     * 
     * @param c The camera for which the directional lighting is to be configured.
     * @param shaders The shaders to be used for rendering.
     * @param lightsCount The count of lights in the scene.
     * 
     * @note This function is called by the render loop to configure the lighting for the specified camera.
     */

    void configure_lighting(Camera *c, WorldShaders *shaders, u8 *lightsCount) {

        // Lights and shadows
        mat4 lightProjection;
        mat4 lightView = GLM_MAT4_IDENTITY_INIT;

        size_t storageBufferIndex;

        f32 near_plane = -50.0f, far_plane = 200.0f;
        glm_ortho(-near_plane, near_plane, -near_plane, near_plane, near_plane, far_plane, lightProjection);
        vec3 dir = {1.0, 0.0, 0.0};

        glm_vec3_rotate(dir, to_radians(this->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_vec3_rotate(dir, to_radians(this->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_vec3_rotate(dir, to_radians(this->rot[2]), (vec3){0.0f, 0.0f, 1.0f});

        vec3 lightPos   = {c->pos[0], c->pos[1], c->pos[2]};
        vec3 lightFront = {dir[0], dir[1], dir[2]};
        vec3 lightUp    = {0.0f, 1.0f,  0.0f};
        vec3 lightB;
        glm_vec3_sub(lightPos, lightFront, lightB);
        glm_lookat(lightPos, lightB, lightUp, lightView);

        storageBufferIndex = lightsCount[DIRECTIONAL_LIGHT]*sizeof(mat4)+0*sizeof(mat4);
        lightsCount[DIRECTIONAL_LIGHT]++;



        SUPER(configure_lighting, shaders, lightView, lightProjection, storageBufferIndex);
    }
    
}
