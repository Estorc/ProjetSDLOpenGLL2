/**
 * @file
 * @brief This file contains the implementation of the PointLight class.
 *
 * This source file contains the implementation details for the PointLight class,
 * which is used to represent a point light source in a 3D scene. The PointLight
 * class provides functionalities to set up and manage the properties of a point
 * light, such as its position, color, intensity, and attenuation factors.
 *
 * The PointLight class is designed to be used in conjunction with OpenGL to
 * render realistic lighting effects in a 3D environment.
 *
 * @author Loup Picault
 * @date 2023-10-10
 */

#include <raptiquax.h>
#include <classes/classes.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <storage/node.h>
#include <render/lighting.h>
#include <render/depth_map.h>
#include <buffer.h>

/**
 * @ingroup Classes Classes
 * @{
 */
class PointLight : public Light {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the PointLight class.
     *
     * This function initializes a PointLight object with the specified parameters.
     *
     * @param r The red component of the light color.
     * @param g The green component of the light color.
     * @param b The blue component of the light color.
     * @param bias The bias value for the light.
     * @param size The size of the light.
     * @param constant The constant attenuation factor.
     * @param linear The linear attenuation factor.
     * @param quadratic The quadratic attenuation factor.
     */
    void constructor(float r, float g, float b, float bias, float size, float constant, float linear, float quadratic) {
        this->type = __type__;

        PointLight *pointLight;
        pointLight = malloc(sizeof(PointLight));
        POINTER_CHECK(pointLight);

        Game.buffers->lightingBuffer.length++;

        pointLight->color[0] = r;
        pointLight->color[1] = g;
        pointLight->color[2] = b;
        pointLight->bias = bias;
        pointLight->size = size;
        pointLight->constant = constant;
        pointLight->linear = linear;
        pointLight->quadratic = quadratic;

        this->object = pointLight;
        SUPER(initialize_node);
        SUPER(init_light);
    }

    /**
     * @brief Updates the position, rotation, and scale of a point light, and increments the lights count.
     *
     * This function updates the position, rotation, and scale of a point light based on the provided delta time.
     * It also increments the count of active lights.
     *
     * @param pos Vec3 structure representing the position of the point light.
     * @param rot Vec3 structure representing the rotation of the point light.
     * @param scale Vec3 structure representing the scale of the point light.
     * @param delta The time delta used to update the point light's properties.
     * @param lightsCount Pointer to an unsigned 8-bit integer representing the count of active lights.
     */
    void update(vec3 pos, vec3 rot, vec3 scale, double delta, u8 *lightsCount) {
        UNUSED(delta);
        if (!(this->flags & NODE_LIGHT_ACTIVE)) return;
        PointLight *pointLight = (PointLight *) this->object;

        this::update_global_position(pos, rot, scale);

        const char uniforms[8][20] = {
            "].position",
            "].color",
            "].bias",
            "].size",
            "].constant",
            "].linear",
            "].quadratic",
            "].index"
        };

        char uniformsName[8][50];

        for (int i = 0; i < 8; i++) {
            strcpy(uniformsName[i], "pointLights[");
            sprintf(uniformsName[i] + strlen(uniformsName[i]), "%d", lightsCount[POINT_LIGHT]);
            strcat(uniformsName[i], uniforms[i]);
        }

        for (int i = 0; i < Game.memoryCaches->shadersCount; i++) {
            use_shader(Game.memoryCaches->shaderCache[i].shader);
            set_shader_vec3(Game.memoryCaches->shaderCache[i].shader, uniformsName[0], this->globalPos);
            set_shader_vec3(Game.memoryCaches->shaderCache[i].shader, uniformsName[1], pointLight->color);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[2], pointLight->bias);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[3], pointLight->size);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[4], pointLight->constant);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[5], pointLight->linear);
            set_shader_float(Game.memoryCaches->shaderCache[i].shader, uniformsName[6], pointLight->quadratic);
            set_shader_int(Game.memoryCaches->shaderCache[i].shader, uniformsName[7], lightsCount[DIRECTIONAL_LIGHT] + lightsCount[POINT_LIGHT]*6 + lightsCount[SPOT_LIGHT]);
        }
        Game.buffers->lightingBuffer.lightings[Game.buffers->lightingBuffer.index++] = this;
        lightsCount[POINT_LIGHT]++;
    }    

    /**
     * @brief Retrieves the settings data for the point light.
     *
     * This function populates the provided pointer with the settings data
     * and sets the length of the data.
     *
     * @param[out] ptr A pointer to a pointer to a pointer where the settings data will be stored.
     * @param[out] length A pointer to an integer where the length of the settings data will be stored.
     */
    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        PointLight *pointLight = (PointLight *) this->object;
        void *data[] = {
            "rgb", "Color : ", &pointLight->color,
            "float", "Bias : ", &pointLight->bias,
            "float", "Size : ", &pointLight->size,
            "float", "Constant : ", &pointLight->constant,
            "float", "Linear : ", &pointLight->linear,
            "float", "Quadratic : ", &pointLight->quadratic
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the specified file and loads it into the
     * appropriate structures or variables. The file pointer should be valid and
     * opened in the appropriate mode for reading.
     *
     * @param file A pointer to the file from which data will be loaded.
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
     * @brief Saves the state of the point light to a file.
     *
     * This function writes the current state of the point light to the specified
     * file. The state includes all relevant properties of the point light that
     * need to be persisted.
     *
     * @param file A pointer to the FILE object where the point light state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        PointLight *pointLight = (PointLight*) this->object;
        fprintf(file, "(%g,%g,%g,%g,%g,%g,%g,%g)",
            pointLight->color[0], pointLight->color[1], pointLight->color[2], 
            pointLight->bias,
            pointLight->size,
            pointLight->constant,
            pointLight->linear,
            pointLight->quadratic
        );
    }

    /**
     * @brief Configures the lighting
     * 
     * This function configures the lighting.
     * 
     * @param c The camera for which the lighting is to be configured.
     * @param shaders The shaders to be used for rendering.
     * @param lightsCount The number of lights in the scene.
     * @param pointLightId The ID of the point light.
     */
    void configure_lighting(Camera *c, WorldShaders *shaders, DepthMap *depthMap, u8 *lightsCount, int pointLightId) {

        UNUSED(c);
        // Lights and shadows
        mat4 lightProjection;
        mat4 lightView = GLM_MAT4_IDENTITY_INIT;

        size_t storageBufferIndex;

        f32 near_plane = 1.0f, far_plane = 8000.0f;
        glm_perspective(PI/2.0f, 1.0f, near_plane, far_plane, lightProjection);

        vec3 directions[6] = {
            { 1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            { 0.0f, 1.0f, 0.0f},
            { 0.0f,-1.0f, 0.0f},
            { 0.0f, 0.0f,-1.0f},
            { 0.0f, 0.0f, 1.0f},
        };

        vec3 upVectors[6] = {
            { 0.0f, -1.0f,  0.0f},  // +X
            { 0.0f, -1.0f,  0.0f},  // -X
            { 0.0f,  0.0f,  1.0f},  // +Y (top)
            { 0.0f,  0.0f, -1.0f},  // -Y (bottom)
            { 0.0f, -1.0f,  0.0f},  // +Z
            { 0.0f, -1.0f,  0.0f}   // -Z
        };

        vec3 lightPos   = {this->globalPos[0], this->globalPos[1], this->globalPos[2]};
        vec3 lightB;
        glm_vec3_sub(lightPos, directions[pointLightId], lightB);
        glm_lookat(lightPos, lightB, upVectors[pointLightId], lightView);

        storageBufferIndex = (lightsCount[POINT_LIGHT]*6+pointLightId)+(NUM_DIRECTIONAL_LIGHTS);
        if (pointLightId == 5) lightsCount[POINT_LIGHT]++;


        SUPER(configure_lighting, shaders, lightView, lightProjection, storageBufferIndex, depthMap);
    }

}

