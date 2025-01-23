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
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_SPOTLIGHT;
#line 13 "src/classes/nodes/lights/spot_light.class.c"
void __class_method_spotlight_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct SpotLight * spotLight = va_arg(args, struct SpotLight *);(void)this;
        this->object = spotLight;
        this->type = __type__;
        SUPER(initialize_node);
        SUPER(init_light);
}

#line 20 "src/classes/nodes/lights/spot_light.class.c"
void __class_method_spotlight_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * pos = va_arg(args, vec3 *);vec3 * rot = va_arg(args, vec3 *);vec3 * scale = va_arg(args, vec3 *);double  delta = va_arg(args, double );u8 * lightsCount = va_arg(args, u8 *);(void)this;
        IGNORE(delta);
        if (!(this->flags & NODE_LIGHT_ACTIVE)) return;
        SpotLight *spotLight = (SpotLight *) this->object;

        this::update_global_position(pos, rot, scale);

        const char uniforms[11][20] = {
            "].position",
            "].direction",
            "].ambient",
            "].diffuse",
            "].specular",
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

        for (int i = 0; i < memoryCaches.shadersCount; i++) {
            use_shader(memoryCaches.shaderCache[i].shader);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[0], this->globalPos);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[1], dir);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[2], spotLight->ambient);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[3], spotLight->diffuse);
            set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[4], spotLight->specular);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[5], spotLight->constant);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[6], spotLight->linear);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[7], spotLight->quadratic);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[8], spotLight->cutOff);
            set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[9], spotLight->outerCutOff);
            set_shader_int(memoryCaches.shaderCache[i].shader, uniformsName[10], lightsCount[DIRECTIONAL_LIGHT] + lightsCount[POINT_LIGHT]*6 + lightsCount[SPOT_LIGHT]);
        }

        buffers.lightingBuffer.lightings[buffers.lightingBuffer.index++] = this;
        lightsCount[SPOT_LIGHT]++;
}

#line 73 "src/classes/nodes/lights/spot_light.class.c"
void __class_method_spotlight_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
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
        this->type = __type__;
        this::constructor(spotLight);
        this->flags |= NODE_EDITOR_FLAG;
}

#line 106 "src/classes/nodes/lights/spot_light.class.c"
void __class_method_spotlight_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
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


    /**
     * @brief Configures the lighting.
     * 
     * This function configures the lighting.
     * 
     * @param c The camera to be used for rendering.
     * @param shaders The shaders to be used for rendering.
     * @param lightsCount The number of lights in the scene.
     */

#line 132 "src/classes/nodes/lights/spot_light.class.c"
void __class_method_spotlight_configure_lighting(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Camera * c = va_arg(args, Camera *);WorldShaders * shaders = va_arg(args, WorldShaders *);u8 * lightsCount = va_arg(args, u8 *);(void)this;

        IGNORE(c);
        // Lights and shadows
        mat4 lightProjection;
        mat4 lightView = GLM_MAT4_IDENTITY_INIT;

        size_t storageBufferIndex;

        f32 near_plane = 1.0f, far_plane = 200.0f;
        glm_perspective(to_radians(90.0f), SHADOW_WIDTH/SHADOW_HEIGHT, near_plane, far_plane, lightProjection);

        vec3 rot, pos;
        glm_vec3_negate_to(this->globalPos, pos);
        glm_vec3_negate_to(this->globalRot, rot);

        glm_rotate(lightView, to_radians(rot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(lightView, to_radians(rot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(lightView, to_radians(rot[2]), (vec3){0.0f, 0.0f, 1.0f});
        glm_translate(lightView, (vec3){pos[0], pos[1], pos[2]});

        storageBufferIndex = lightsCount[SPOT_LIGHT]*sizeof(mat4)+200*sizeof(mat4);
        lightsCount[SPOT_LIGHT]++;


        SUPER(configure_lighting, shaders, lightView, lightProjection, storageBufferIndex);
}

    
