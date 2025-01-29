#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../render/lighting.h"
#include "../render/depth_map.h"
#include "../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_POINTLIGHT;
#line 13 "src/classes/nodes/lights/point_light.class.c"
void __class_method_pointlight_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct PointLight * pointLight = va_arg(args, struct PointLight *);(void)this;
        this->object = pointLight;
        this->type = __type__;
        SUPER(initialize_node);
        SUPER(init_light);
}

#line 20 "src/classes/nodes/lights/point_light.class.c"
void __class_method_pointlight_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * pos = va_arg(args, vec3 *);vec3 * rot = va_arg(args, vec3 *);vec3 * scale = va_arg(args, vec3 *);double  delta = va_arg(args, double );u8 * lightsCount = va_arg(args, u8 *);(void)this;
        IGNORE(delta);
        if (!(this->flags & NODE_LIGHT_ACTIVE)) return;
        PointLight *pointLight = (PointLight *) this->object;

        this::update_global_position(pos, rot, scale);

        printf("Point light position: %f, %f, %f\n", this->globalPos[0], this->globalPos[1], this->globalPos[2]);

        const char uniforms[8][20] = {
            "].position",
            "].ambient",
            "].diffuse",
            "].specular",
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

        for (int i = 0; i < memoryCaches.shadersCount; i++) {
            use_shader(memoryCaches.shaderCache[i].shader);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[0], this->globalPos);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[1], pointLight->ambient);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[2], pointLight->diffuse);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[3], pointLight->specular);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[4], pointLight->constant);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[5], pointLight->linear);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[6], pointLight->quadratic);
            set_shader_int(memoryCaches.shaderCache[i].shader, uniformsName[7], lightsCount[DIRECTIONAL_LIGHT] + lightsCount[POINT_LIGHT]*6 + lightsCount[SPOT_LIGHT]);
        }
        buffers.lightingBuffer.lightings[buffers.lightingBuffer.index++] = this;
        lightsCount[POINT_LIGHT]++;
}

#line 63 "src/classes/nodes/lights/point_light.class.c"
void __class_method_pointlight_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
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
        this->type = __type__;
        this::constructor(pointLight);
        this->flags |= NODE_EDITOR_FLAG;
}

#line 92 "src/classes/nodes/lights/point_light.class.c"
void __class_method_pointlight_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
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

#line 116 "src/classes/nodes/lights/point_light.class.c"
void __class_method_pointlight_configure_lighting(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Camera * c = va_arg(args, Camera *);WorldShaders * shaders = va_arg(args, WorldShaders *);u8 * lightsCount = va_arg(args, u8 *);int  pointLightId = va_arg(args, int );(void)this;

        IGNORE(c);
        // Lights and shadows
        mat4 lightProjection;
        mat4 lightView = GLM_MAT4_IDENTITY_INIT;

        size_t storageBufferIndex;

        f32 near_plane = 1.0f, far_plane = 8000.0f;
        glm_perspective(to_radians(90.0f), SHADOW_WIDTH/SHADOW_HEIGHT, near_plane, far_plane, lightProjection);

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

        storageBufferIndex = (lightsCount[POINT_LIGHT]*6+pointLightId)*sizeof(mat4)+100*sizeof(mat4);
        if (pointLightId == 5) lightsCount[POINT_LIGHT]++;


        SUPER(configure_lighting, shaders, lightView, lightProjection, storageBufferIndex);
}

    
