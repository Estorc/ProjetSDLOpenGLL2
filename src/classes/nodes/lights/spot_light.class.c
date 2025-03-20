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

/**
 * @ingroup Classes Classes
 * @{
 */
class SpotLight : public Light {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the SpotLight class.
     *
     * This function initializes a SpotLight object with the specified parameters.
     *
     * @param r The red component of the light color.
     * @param g The green component of the light color.
     * @param b The blue component of the light color.
     * @param bias The bias value for the light.
     * @param size The size of the light.
     * @param constant The constant attenuation factor.
     * @param linear The linear attenuation factor.
     * @param quadratic The quadratic attenuation factor.
     * @param cutOff The inner cutoff angle for the spotlight (in degrees).
     * @param outerCutOff The outer cutoff angle for the spotlight (in degrees).
     */
    void constructor(float r, float g, float b, float bias, float size, float constant, float linear, float quadratic, float cutOff, float outerCutOff) {
        this->type = __type__;

        SpotLight *spotLight;
        spotLight = malloc(sizeof(SpotLight));
        POINTER_CHECK(spotLight);

        Game.buffers->lightingBuffer.length++;

        spotLight->color[0] = r;
        spotLight->color[1] = g;
        spotLight->color[2] = b;
        spotLight->bias = bias;
        spotLight->size = size;
        spotLight->constant = constant;
        spotLight->linear = linear;
        spotLight->quadratic = quadratic;
        spotLight->cutOff = cutOff;
        spotLight->outerCutOff = outerCutOff;

        this->object = spotLight;
        SUPER(initialize_node);
        SUPER(init_light);
    }

    /**
     * @brief Updates the state of the spotlight.
     *
     * This function updates the position, rotation, and scale of the spotlight
     * based on the provided delta time and increments the lights count.
     *
     * @param pos Vec3 structure representing the position of the spotlight.
     * @param rot Vec3 structure representing the rotation of the spotlight.
     * @param scale Vec3 structure representing the scale of the spotlight.
     * @param delta The time delta used for updating the spotlight's state.
     * @param lightsCount Pointer to an unsigned 8-bit integer representing the count of lights.
     */
    void update(vec3 pos, vec3 rot, vec3 scale, float delta, u8 *lightsCount) {
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

    /**
     * @brief Retrieves the settings data.
     *
     * This function populates the provided pointer with the settings data and sets the length of the data.
     *
     * @param[out] ptr A pointer to a pointer to a pointer where the settings data will be stored.
     * @param[out] length A pointer to an integer where the length of the settings data will be stored.
     */
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

    /**
     * @brief Loads the SpotLight data from a file.
     *
     * This function reads the SpotLight data from the specified file and initializes the SpotLight object
     * with the loaded data. The file should contain the necessary information to fully describe the SpotLight,
     * including its position, direction, color, intensity, and other relevant properties.
     *
     * @param file A pointer to the file from which the SpotLight data will be loaded.
     */
    void load(FILE *file) {
        vec3 color;
        f32 bias, size, constant, linear, quadratic, cutOff, outerCutOff;
        if (file) {
            fscanf(file,"(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", 
                &color[0], &color[1], &color[2], 
                &bias,
                &size,
                &constant,
                &linear,
                &quadratic,
                &cutOff,
                &outerCutOff
                );
        } else {
            glm_vec3_one(color);
            bias = 0.005f;
            size = 0.0f;
            constant = 1.0f;
            linear = 0.09f;
            quadratic = 0.032f;
            cutOff = 0.01f;
            outerCutOff = 50.0f;
        }

        this::constructor(color[0], color[1], color[2], bias, size, constant, linear, quadratic, cutOff, outerCutOff);
        this->flags |= NODE_EDITOR_FLAG;
    }

    /**
     * @brief Saves the current state of the SpotLight object to a file.
     *
     * This function writes the current state of the SpotLight object to the specified file.
     * The file should be opened in a mode that allows writing before calling this function.
     *
     * @param file A pointer to the FILE object where the state will be saved.
     */
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

