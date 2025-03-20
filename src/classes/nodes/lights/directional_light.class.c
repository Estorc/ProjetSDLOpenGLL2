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

/**
 * @ingroup Classes Classes
 * @{
 */
class DirectionalLight : public Light {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the directional light class.
     *
     * This function initializes a directional light with the specified parameters.
     *
     * @param r Red component of the light color.
     * @param g Green component of the light color.
     * @param b Blue component of the light color.
     * @param bias Bias value for the light.
     * @param size Size of the light.
     * @param constant Constant attenuation factor.
     * @param linear Linear attenuation factor.
     * @param quadratic Quadratic attenuation factor.
     */
    void constructor(float r, float g, float b, float bias, float size, float constant, float linear, float quadratic) {
        this->type = __type__;

        DirectionalLight *directionalLight;
        directionalLight = malloc(sizeof(DirectionalLight));
        POINTER_CHECK(directionalLight);

        Game.buffers->lightingBuffer.length++;

        directionalLight->color[0] = r;
        directionalLight->color[1] = g;
        directionalLight->color[2] = b;
        directionalLight->bias = bias;
        directionalLight->size = size;
        directionalLight->constant = constant;
        directionalLight->linear = linear;
        directionalLight->quadratic = quadratic;

        this->object = directionalLight;
        SUPER(initialize_node);
        SUPER(init_light);
    }

    /**
     * @brief Updates the state of a directional light.
     *
     * This function updates the position, rotation, and scale of a directional light
     * based on the given delta time and updates the count of active lights.
     *
     * @param pos Vec3 structure representing the position of the light.
     * @param rot Vec3 structure representing the rotation of the light.
     * @param scale Vec3 structure representing the scale of the light.
     * @param delta The time delta used for updating the light's state.
     * @param lightsCount Pointer to an unsigned 8-bit integer representing the count of active lights.
     */
    void update(vec3 pos, vec3 rot, vec3 scale, double delta, u8 *lightsCount) {
        UNUSED(delta);
        if (!(this->flags & NODE_LIGHT_ACTIVE)) return;
        DirectionalLight *directionalLight = (DirectionalLight *) this->object;

        this::update_global_position(pos, rot, scale);

        const char uniforms[9][20] = {
            "].position",
            "].direction",
            "].color",
            "].bias",
            "].size",
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

        for (int i = 0; i < Game.memoryCaches->shadersCount; i++) {
            use_shader(Game.memoryCaches->shaderCache[i].shader);
            set_shader_vec3(Game.memoryCaches->shaderCache[i].shader, uniformsName[0], this->globalPos);
            set_shader_vec3(Game.memoryCaches->shaderCache[i].shader, uniformsName[1], dir);
            set_shader_vec3(Game.memoryCaches->shaderCache[i].shader, uniformsName[2], directionalLight->color);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[3], directionalLight->bias);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[4], directionalLight->size);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[5], directionalLight->constant);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[6], directionalLight->linear);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[7], directionalLight->quadratic);
            set_shader_int(Game.memoryCaches->shaderCache[i].shader, uniformsName[8], lightsCount[DIRECTIONAL_LIGHT] + lightsCount[POINT_LIGHT]*6 + lightsCount[SPOT_LIGHT]);
        }

        Game.buffers->lightingBuffer.lightings[Game.buffers->lightingBuffer.index++] = this;
        lightsCount[DIRECTIONAL_LIGHT]++;
    }   

    /**
     * @brief Retrieves the settings data for the directional light.
     *
     * This function populates the provided pointer with the settings data
     * and sets the length of the data.
     *
     * @param[out] ptr A pointer to a pointer to a pointer where the settings data will be stored.
     *                 The function will allocate memory for the settings data and set this pointer
     *                 to point to the allocated memory.
     * @param[out] length A pointer to an integer where the length of the settings data will be stored.
     *                    The function will set this integer to the length of the settings data.
     */
    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        DirectionalLight *directionalLight = (DirectionalLight*) this->object;
        void *data[] = {
            "rgb", "Color : ", &directionalLight->color,
            "float", "Bias : ", &directionalLight->bias,
            "float", "Size : ", &directionalLight->size,
            "float", "Constant : ", &directionalLight->constant,
            "float", "Linear : ", &directionalLight->linear,
            "float", "Quadratic : ", &directionalLight->quadratic
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the given file and loads it into the appropriate
     * structures or variables. The file pointer must be valid and opened in the 
     * appropriate mode for reading.
     *
     * @param file A pointer to the FILE object that identifies the file to read from.
     */
    void load(FILE *file) {
        vec3 color;
        f32 bias, size, constant, linear, quadratic;

        if (file) {
            fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g)\n", 
                &color[0], &color[1], &color[2], 
                &bias,
                &size,
                &constant,
                &linear,
                &quadratic
                );
        } else {
            glm_vec3_one(color);
            bias = 0.005f;
            size = 0.0f;
            constant = 1.0f;
            linear = 0.09f;
            quadratic = 0.032f;
        }

        this::constructor(color[0], color[1], color[2], bias, size, constant, linear, quadratic);
        this->flags |= NODE_EDITOR_FLAG;
    }

    /**
     * @brief Saves the current state of the directional light to a file.
     *
     * This function writes the current state of the directional light object to the specified file.
     *
     * @param file A pointer to the FILE object where the state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        DirectionalLight *directionalLight = (DirectionalLight*) this->object;
        fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g)",
            directionalLight->color[0], directionalLight->color[1], directionalLight->color[2], 
            directionalLight->bias,
            directionalLight->size,
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
    void configure_lighting(Camera *c, WorldShaders *shaders, DepthMap *depthMap, u8 *lightsCount) {

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

        storageBufferIndex = lightsCount[DIRECTIONAL_LIGHT];
        lightsCount[DIRECTIONAL_LIGHT]++;



        SUPER(configure_lighting, shaders, lightView, lightProjection, storageBufferIndex, depthMap);
    }
    
}

