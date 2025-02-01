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
        IGNORE(delta);
        if (!(this->flags & NODE_LIGHT_ACTIVE)) return;
        DirectionalLight *directionalLight = (DirectionalLight *) this->object;

        this::update_global_position(pos, rot, scale);

        const char uniforms[9][20] = {
            "].position",
            "].direction",
            "].ambient",
            "].diffuse",
            "].specular",
            "].constant",
            "].linear",
            "].quadratic",
            "].index"
        };

        char uniformsName[9][50];

        for (int i = 0; i < 9; i++) {
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
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[2], directionalLight->ambient);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[3], directionalLight->diffuse);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[4], directionalLight->specular);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[5], directionalLight->constant);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[6], directionalLight->linear);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[7], directionalLight->quadratic);
            set_shader_int(memoryCaches.shaderCache[i].shader, uniformsName[8], lightsCount[DIRECTIONAL_LIGHT] + lightsCount[POINT_LIGHT]*6 + lightsCount[SPOT_LIGHT]);
        }

        buffers.lightingBuffer.lightings[buffers.lightingBuffer.index++] = this;
        lightsCount[DIRECTIONAL_LIGHT]++;
    }   



    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        DirectionalLight *directionalLight = (DirectionalLight*) this->object;
        void *data[] = {
            "rgb", "Ambient : ", &directionalLight->ambient,
            "rgb", "Diffuse : ", &directionalLight->diffuse,
            "rgb", "Specular : ", &directionalLight->specular,
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
        this->type = __type__;
        this::constructor(directionalLight);
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
