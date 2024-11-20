#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;

#define DIR_LIGHTS_MAX 5
#define POINT_LIGHTS_MAX 5
#define SPOT_LIGHTS_MAX 5

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosDirLightSpace[DIR_LIGHTS_MAX];
    vec4 FragPosPointLightSpace[12];
    vec4 FragPosSpotLightSpace[SPOT_LIGHTS_MAX];
    vec3 viewPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    flat int pointLightsNum;
    flat int dirLightsNum;
    flat int spotLightsNum;
    mat3 TBN;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 viewPos;

uniform int pointLightsNum;
uniform int dirLightsNum;
uniform int spotLightsNum;

uniform mat4 dirLightSpaceMatrix[DIR_LIGHTS_MAX];
uniform mat4 pointLightSpaceMatrix[12];
uniform mat4 spotLightSpaceMatrix[SPOT_LIGHTS_MAX];

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;

    for(int i = 0; i < dirLightsNum && i < DIR_LIGHTS_MAX; i++) {
        vs_out.FragPosDirLightSpace[i] = dirLightSpaceMatrix[i] * vec4(vs_out.FragPos, 1.0);
    }
    // phase 2: point lights
    for(int i = 0; i < pointLightsNum && i < POINT_LIGHTS_MAX; i++) {
        for (int j = 0; j < 6; j++)
            vs_out.FragPosPointLightSpace[i*6+j] = pointLightSpaceMatrix[i*6+j] * vec4(vs_out.FragPos, 1.0);
    }
    // phase 3: spot light
    for(int i = 0; i < spotLightsNum && i < SPOT_LIGHTS_MAX; i++) {
        vs_out.FragPosSpotLightSpace[i] = spotLightSpaceMatrix[i] * vec4(vs_out.FragPos, 1.0);
    }

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    vs_out.TBN = mat3(T, B, N);
    vs_out.viewPos = viewPos;
    vs_out.TangentViewPos  = vs_out.TBN * viewPos;
    vs_out.TangentFragPos  = vs_out.TBN * vs_out.FragPos;
    vs_out.pointLightsNum = pointLightsNum;
    vs_out.dirLightsNum = dirLightsNum;
    vs_out.spotLightsNum = spotLightsNum;
}