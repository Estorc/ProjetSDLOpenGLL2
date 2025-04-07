#version 430 core
// https://imanolfotia.com/blog/1


struct DirLight {
    vec3 position;
    vec3 direction;

    float size;
    float bias;
	
    vec3 color;
    int index;
};

struct PointLight {
    vec3 position;

    float size;
    float bias;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 color;
    int index;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float size;
    float bias;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 color;
    int index;     
};


uniform sampler2D ColorBuffer; 
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gExtraComponents;
uniform sampler2D ssao;
out vec4 outColor;
uniform sampler2DArrayShadow shadowMap;
uniform samplerBuffer lightMatrixBuffer;
uniform bool shadowCastActive;
uniform bool ssaoActive;

uniform int pointLightsNum;
uniform int dirLightsNum;
uniform int spotLightsNum;

#define DIR_LIGHTS_MAX 10
#define POINT_LIGHTS_MAX 10
#define SPOT_LIGHTS_MAX 10

uniform DirLight dirLights[DIR_LIGHTS_MAX];
uniform PointLight pointLights[POINT_LIGHTS_MAX];
uniform SpotLight spotLights[SPOT_LIGHTS_MAX];

uniform mat4 projection;
uniform mat4 view;
uniform mat4 invView;
uniform mat4 invProjection;
uniform vec3 viewPos;

noperspective in vec2 TexCoords;

const float PI = 3.14159265359;

float metallic = 0.0;
float roughness = 0.5;
float shininess = 0.0;
vec3 albedo;  
const float gamma = 1.2; 
float ao = 0.03; // ambient occlusion
vec3 F0 = vec3(0.04); 

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int index, float lightBias, float distanceFromCamera, vec2 texelSize);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int lightIndex, float distanceFromCamera, vec2 shadowMapTexelSize);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int lightIndex, float distanceFromCamera, vec2 shadowMapTexelSize);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int lightIndex, float distanceFromCamera, vec2 shadowMapTexelSize);
float DistributionGGX(vec3 N, vec3 H, float roughness, float size);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
float FresnelSchlickF0(float ior);
vec3 fresnelSchlick(float cosTheta, vec3 F0);


void main()
{
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec4 completeFragPos = (invView * texture(gPosition, TexCoords));
    vec3 fragPos = completeFragPos.xyz;
    float depth = completeFragPos.w;
    albedo = texture(ColorBuffer, TexCoords).rgb;
    vec3 viewDir = normalize(viewPos - fragPos);

    vec4 MetallicEmmissive = texture(gExtraComponents, TexCoords);
    metallic = MetallicEmmissive.r;
    roughness = MetallicEmmissive.g;
    shininess = MetallicEmmissive.b;
    float intensity = MetallicEmmissive.a;

    if (intensity >= 1.0) {
        outColor = vec4(albedo, 1.0);
        return;
    }

    if (ssaoActive) ao = texture(ssao, TexCoords).r;

    F0 = vec3(FresnelSchlickF0(shininess));
    F0 = mix(F0, albedo, metallic);

    float distanceFromCamera = length(fragPos - viewPos);

    /*if (TexCoords.x < 0.5 && TexCoords.y < 0.5) {
        outColor = texture(gFinalImage, TexCoords);
        return;
    }
    if (TexCoords.x > 0.5 && TexCoords.y > 0.5) {
        outColor = texture(gPosition, TexCoords);
        return;
    }
    if (TexCoords.x < 0.5 && TexCoords.y > 0.5) {
        outColor = texture(gNormal, TexCoords);
        return;
    }
    if (TexCoords.x > 0.5 && TexCoords.y < 0.5) {
        outColor = texture(gExtraComponents, TexCoords);
        return;
    }*/


    vec2 shadowMapTexelSize = textureSize(shadowMap, 0).xy;
    vec3 Lo = vec3(0); // reflectance equation
    // phase 1: directional lighting
    for(int i = 0; i < dirLightsNum && i < DIR_LIGHTS_MAX; i++) {
        Lo += CalcDirLight(dirLights[i], normal, fragPos, viewDir, i, distanceFromCamera, shadowMapTexelSize);
    }
    // phase 2: point lights
    for(int i = 0; i < pointLightsNum && i < POINT_LIGHTS_MAX; i++) {
        Lo += CalcPointLight(pointLights[i], normal, fragPos, viewDir, i, distanceFromCamera, shadowMapTexelSize);  
    }  
    // phase 3: spot light
    for(int i = 0; i < spotLightsNum && i < SPOT_LIGHTS_MAX; i++) {
        Lo += CalcSpotLight(spotLights[i], normal, fragPos, viewDir, i, distanceFromCamera, shadowMapTexelSize);
    }  
    
    vec3 ambient = max(vec3(0.03) * albedo * (ao - (distanceFromCamera*distanceFromCamera)/200.0), vec3(0.0)); // ambient occlusion
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/gamma));  

    outColor = mix(vec4(color, 1.0), vec4(albedo, 1.0), intensity); // mix with albedo to get the final color
}


const vec2 poissonDisk[8] = vec2[8](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(0.53909567, -0.13220742),
    vec2(-0.10630698, 0.15260516),
    vec2(0.48779802, 0.47373719),
    vec2(-0.31213645, -0.80381258)
);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int index, float lightBias, float distanceFromCamera, vec2 texelSize) {
    
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.z < 0.0)
        return 0.0;

    // get depth of current fragment from light's perspective
    // check whether current frag pos is in shadow
    float bias = max(lightBias * (1.0 - dot(normal, lightDir)), lightBias); 
    bias *= (1.0 / (1.0 + length(fragPosLightSpace.xyz))); 

    float shadow = 0.0;
    for(int i = 0; i <= 7; ++i) {
        shadow += 1.0 - texture(shadowMap, vec4(vec2(projCoords.xy + poissonDisk[i] / texelSize), index, projCoords.z - bias));  
    }
    shadow /= float(8); // Apromixation for performance and quality

    return shadow;
}



// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 N /* aka Normal */, vec3 fragPos, vec3 V /* aka ViewDir */, int lightIndex, float distanceFromCamera, vec2 shadowMapTexelSize)
{
    vec3 L = normalize(light.direction);
    vec3 H = normalize(V + L);
    if (dot(N, L) < 0.0) return vec3(0.0); // avoid useless calculations


    // calculate shadow
    float shadow = 0.0;
    if (shadowCastActive) {
        mat4 lightMatrix = mat4(
            texelFetch(lightMatrixBuffer, lightIndex * 4 + 0 + 0),
            texelFetch(lightMatrixBuffer, lightIndex * 4 + 1 + 0),
            texelFetch(lightMatrixBuffer, lightIndex * 4 + 2 + 0),
            texelFetch(lightMatrixBuffer, lightIndex * 4 + 3 + 0)
        );
        shadow = ShadowCalculation(lightMatrix * vec4(fragPos, 1.0), N, light.position, light.index, light.bias, distanceFromCamera, shadowMapTexelSize);
    }
    if (shadow >= 1.0) return vec3(0.0); // avoid useless calculations


    // combine results
    vec3 radiance = light.color;
    
    float NDF = DistributionGGX(N, H, roughness, light.size);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);  

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	 

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    return (kD * albedo / PI + specular) * radiance * NdotL * (1.0 - shadow); 
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 N /* aka Normal */, vec3 fragPos, vec3 V /* aka ViewDir */, int lightIndex, float distanceFromCamera, vec2 shadowMapTexelSize)
{
    vec3 L = normalize(light.position - fragPos); // aka lightDir
    vec3 H = normalize(V + L);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + max(light.quadratic, 0.032) * (distance * distance));  
    if (attenuation < 0.001) return vec3(0.0); // avoid useless calculations


    // calculate shadow
    float shadow = 0.0;
    if (shadowCastActive) for (int j = 0; j < 6; j++) {
        mat4 lightMatrix = mat4(
            texelFetch(lightMatrixBuffer, (lightIndex * 6 + j) * 4 + 0 + DIR_LIGHTS_MAX * 4),
            texelFetch(lightMatrixBuffer, (lightIndex * 6 + j) * 4 + 1 + DIR_LIGHTS_MAX * 4),
            texelFetch(lightMatrixBuffer, (lightIndex * 6 + j) * 4 + 2 + DIR_LIGHTS_MAX * 4),
            texelFetch(lightMatrixBuffer, (lightIndex * 6 + j) * 4 + 3 + DIR_LIGHTS_MAX * 4)
        );
        shadow += ShadowCalculation(lightMatrix * vec4(fragPos, 1.0), N, light.position, light.index+j, light.bias, distanceFromCamera, shadowMapTexelSize);
    }
    if (shadow >= 1.0) return vec3(0.0); // avoid useless calculations

    vec3 radiance = light.color * attenuation;

    float NDF = DistributionGGX(N, H, roughness, light.size);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);  

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	 

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    return (kD * albedo / PI + specular) * radiance * NdotL * (1.0 - shadow); 
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 N /* aka Normal */, vec3 fragPos, vec3 V /* aka ViewDir */, int lightIndex, float distanceFromCamera, vec2 shadowMapTexelSize)
{
    vec3 L = normalize(light.position - fragPos); // aka lightDir
    vec3 H = normalize(V + L);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + max(light.linear, 0.032) * distance + max(light.quadratic, 0.032) * (distance * distance));    
    if (attenuation < 0.001) return vec3(0.0); // avoid useless calculations

    // spotlight intensity
    float theta = dot(L, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    if (theta < light.outerCutOff) return vec3(0.0); // avoid useless calculations


    // calculate shadow
    float shadow = 0.0;
    if (shadowCastActive) {
        mat4 lightMatrix = mat4(
            texelFetch(lightMatrixBuffer, lightIndex * 4 + 0 + (DIR_LIGHTS_MAX + POINT_LIGHTS_MAX*6) * 4),
            texelFetch(lightMatrixBuffer, lightIndex * 4 + 1 + (DIR_LIGHTS_MAX + POINT_LIGHTS_MAX*6) * 4),
            texelFetch(lightMatrixBuffer, lightIndex * 4 + 2 + (DIR_LIGHTS_MAX + POINT_LIGHTS_MAX*6) * 4),
            texelFetch(lightMatrixBuffer, lightIndex * 4 + 3 + (DIR_LIGHTS_MAX + POINT_LIGHTS_MAX*6) * 4)
        );
        shadow = ShadowCalculation(lightMatrix * vec4(fragPos, 1.0), N, light.position, light.index, light.bias, distanceFromCamera, shadowMapTexelSize);
    }
    if (shadow >= 1.0) return vec3(0.0); // avoid useless calculations


    vec3 radiance = light.color * intensity * attenuation;

    float NDF = DistributionGGX(N, H, roughness, light.size);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);  

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	 

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    return (kD * albedo / PI + specular) * radiance * NdotL * (1.0 - shadow); 
}


float DistributionGGX(vec3 N, vec3 H, float roughness, float size)
{
    float a      = roughness*roughness+size/100.0; // Automatize this value
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = pow(denom, 2.0+size/10.0);
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float FresnelSchlickF0(float ior) {
    return pow((ior - 1.0) / (ior + 1.0), 2.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  