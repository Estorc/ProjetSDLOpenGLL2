#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
    float parallax;
    float shininess;
    float metallic;
    float roughness;    
}; 

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D parallaxMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2DArray shadowMap;

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

#define DIR_LIGHTS_MAX 10
#define POINT_LIGHTS_MAX 12
#define SPOT_LIGHTS_MAX 10

layout(std140) uniform LightMatrices {
    mat4 dirLightSpaceMatrix[DIR_LIGHTS_MAX];
    mat4 pointLightSpaceMatrix[POINT_LIGHTS_MAX];
    mat4 spotLightSpaceMatrix[SPOT_LIGHTS_MAX];
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 viewPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    mat3 TBN;
} fs_in;

uniform int pointLightsNum;
uniform int dirLightsNum;
uniform int spotLightsNum;

uniform bool diffuseMapActive;
uniform bool normalMapActive;
uniform bool parallaxMapActive;
uniform bool metallicMapActive;
uniform bool roughnessMapActive;
uniform bool shadowCastActive;
uniform Material material;

uniform DirLight dirLights[DIR_LIGHTS_MAX];
uniform PointLight pointLights[POINT_LIGHTS_MAX];
uniform SpotLight spotLights[SPOT_LIGHTS_MAX];

const float PI = 3.14159265359;
const int SHADOW_QUALITY = 1;

float metallic = 0.0;
float roughness = 0.5;
vec3 albedo;
const float ao = 0.1;   
const float gamma = 1.2; 
vec3 F0 = vec3(0.04); 


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);




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





vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    float heightScale = material.parallax;
    // number of depth layers
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (From vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; // * v.z and not / v.z, strange
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(parallaxMap, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue) {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(parallaxMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // Relief Parallax Mapping

    // decrease shift and height of layer by half
    deltaTexCoords /= 2;
    layerDepth /= 2;

    // return to the mid point of previous layer
    currentTexCoords += deltaTexCoords;
    currentLayerDepth -= layerDepth;

    // binary search to increase precision of Steep Paralax Mapping
    const int numSearches = 5;
    for(int i = 0; i < numSearches; ++i) {
        // decrease shift and height of layer by half
        deltaTexCoords /= 2;
        layerDepth /=2;
        
        // new depth from heightmap
        currentDepthMapValue = texture(parallaxMap, currentTexCoords).r;

        // shift along or aginas vector ViewDir
        if(currentDepthMapValue > currentLayerDepth) {
            currentTexCoords -= deltaTexCoords;
            currentLayerDepth += layerDepth;
        } else {
            currentTexCoords += deltaTexCoords;
            currentLayerDepth -= layerDepth;
        }
    }

    return currentTexCoords;
}



float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int index, float lightBias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, vec3(projCoords.xy, index)).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(lightBias * (1.0 - dot(normal, lightDir)), lightBias);  

    float shadow = 0.0;
    if (SHADOW_QUALITY == 0) return currentDepth - bias > texture(shadowMap, vec3(projCoords.xy, index)).r ? 1.0 : 0.0; 
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0).xy;
    for(int x = -SHADOW_QUALITY; x <= SHADOW_QUALITY; ++x)
    {
        for(int y = -SHADOW_QUALITY; y <= SHADOW_QUALITY; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(vec2(projCoords.xy + vec2(x, y) * texelSize), index)).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= (float(SHADOW_QUALITY)*1.25)*(float(SHADOW_QUALITY)*1.25);

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


void main()
{    

    F0 = vec3(FresnelSchlickF0(material.shininess));
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos);
    vec2 texCoords;
    vec3 tangentViewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    if (parallaxMapActive) {
        texCoords = ParallaxMapping(fs_in.TexCoords,  tangentViewDir);
        /*if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;*/
    } else {
        texCoords = fs_in.TexCoords;
    }


    if (metallicMapActive) {
        metallic = texture(metallicMap, texCoords).r;
    } else {
        metallic = material.metallic;
    }
    if (roughnessMapActive) {
        roughness = texture(roughnessMap, texCoords).r;
    } else {
        roughness = material.roughness;
    }

    vec4 tex;
    if (diffuseMapActive) {
        tex = texture(diffuseMap, texCoords);
        if (tex.a < 0.1)
            discard;
    } else {
        tex = vec4(material.diffuse, 1.0);
    }
    albedo = tex.rgb;

    F0 = mix(F0, albedo, metallic);


    vec3 normal = fs_in.Normal;
    if (normalMapActive) {
        normal = texture(normalMap, texCoords).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(transpose(fs_in.TBN) * normal);
    }
    normal = normalize(normal); 
    //rgbnormal = normal * 0.5 + 0.5; // transforms from [-1,1] to [0,1]  

    vec3 Lo = vec3(0); // reflectance equation
    // phase 1: directional lighting
    for(int i = 0; i < dirLightsNum && i < DIR_LIGHTS_MAX; i++) {
        float shadow = 0.0;
        if (shadowCastActive) shadow = ShadowCalculation(dirLightSpaceMatrix[i] * vec4(fs_in.FragPos, 1.0), normal, dirLights[i].position, dirLights[i].index, dirLights[i].bias);
        Lo += CalcDirLight(dirLights[i], normal, fs_in.FragPos, viewDir, (shadowCastActive) ? min(shadow, 1.0) : 0.0);
    }
    // phase 2: point lights
    for(int i = 0; i < pointLightsNum && i < POINT_LIGHTS_MAX; i++) {
        float shadow = 0.0;
        if (shadowCastActive) for (int j = 0; j < 6; j++)
            shadow += ShadowCalculation(pointLightSpaceMatrix[i*6+j] * vec4(fs_in.FragPos, 1.0), normal, pointLights[i].position, pointLights[i].index+j, pointLights[i].bias);
        Lo += CalcPointLight(pointLights[i], normal, fs_in.FragPos, viewDir, (shadowCastActive) ? min(shadow, 1.0) : 0.0);  
    }  
    // phase 3: spot light
    for(int i = 0; i < spotLightsNum && i < SPOT_LIGHTS_MAX; i++) {
        float shadow = 0.0;
        if (shadowCastActive) shadow = ShadowCalculation(spotLightSpaceMatrix[i] * vec4(fs_in.FragPos, 1.0), normal, spotLights[i].position, spotLights[i].index, spotLights[i].bias);
        Lo += CalcSpotLight(spotLights[i], normal, fs_in.FragPos, viewDir, (shadowCastActive) ? min(shadow, 1.0) : 0.0);
    }  
    
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/gamma));  

    FragColor = vec4(color, 1.0);

    
    //vec3 debugColor = tangentViewDir * 0.5 + 0.5; // Map [-1, 1] range to [0, 1]
    //FragColor = vec4(debugColor, 1.0); // Output as RGB for debugging
    //FragColor.rgb = normal;
}


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 N /* aka Normal */, vec3 fragPos, vec3 V /* aka ViewDir */, float shadow)
{
    vec3 L = normalize(light.direction);
    vec3 H = normalize(V + L);
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
vec3 CalcPointLight(PointLight light, vec3 N /* aka Normal */, vec3 fragPos, vec3 V /* aka ViewDir */, float shadow)
{
    vec3 L = normalize(light.position - fragPos); // aka lightDir
    vec3 H = normalize(V + L);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + max(light.quadratic, 0.032) * (distance * distance));    
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
vec3 CalcSpotLight(SpotLight light, vec3 N /* aka Normal */, vec3 fragPos, vec3 V /* aka ViewDir */, float shadow)
{
    vec3 L = normalize(light.position - fragPos); // aka lightDir
    vec3 H = normalize(V + L);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + max(light.linear, 0.032) * distance + max(light.quadratic, 0.032) * (distance * distance));    
    // spotlight intensity
    float theta = dot(L, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

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