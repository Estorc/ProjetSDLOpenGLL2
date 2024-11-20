#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
    float shininess;
}; 

struct DirLight {
    vec3 position;
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int index;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int index;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  
    int index;     
};

#define DIR_LIGHTS_MAX 5
#define POINT_LIGHTS_MAX 5
#define SPOT_LIGHTS_MAX 5

in VS_OUT {
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
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;
uniform sampler2DArray shadowMap;

uniform bool normalMapActive;
uniform bool displacementMapActive;
uniform Material material;

uniform DirLight dirLights[DIR_LIGHTS_MAX];
uniform PointLight pointLights[POINT_LIGHTS_MAX];
uniform SpotLight spotLights[SPOT_LIGHTS_MAX];


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 

const float heightScale = 2.5;
 // number of depth layers
 const float minLayers = 8.0;
 const float maxLayers = 32.0;
 float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
 // calculate the size of each layer
 float layerDepth = 1.0 / numLayers;
 // depth of current layer
 float currentLayerDepth = 0.0;
 // the amount to shift the texture coordinates per layer (From vector P)
 vec2 P = viewDir.xy * viewDir.z * heightScale; // * v.z and not / v.z, strange
 vec2 deltaTexCoords = P / numLayers;

 // get initial values
 vec2 currentTexCoords = texCoords;
 float currentDepthMapValue = texture(displacementMap, currentTexCoords).r;
 
 while(currentLayerDepth < currentDepthMapValue)
 {
  // shift texture coordinates along direction of P
  currentTexCoords -= deltaTexCoords;
  // get depthmap value at current texture coordinates
  currentDepthMapValue = texture(displacementMap, currentTexCoords).r;
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
 for(int i = 0; i < numSearches; ++i)
 {
  // decrease shift and height of layer by half
  deltaTexCoords /= 2;
  layerDepth /=2;
  
  // new depth from heightmap
  currentDepthMapValue = texture(displacementMap, currentTexCoords).r;

  // shift along or aginas vector ViewDir
  if(currentDepthMapValue > currentLayerDepth)
  {
   currentTexCoords -= deltaTexCoords;
   currentLayerDepth += layerDepth;
  }
  else
  {
   currentTexCoords += deltaTexCoords;
   currentLayerDepth -= layerDepth;
  }
 }

 return currentTexCoords;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int index)
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
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.05);  

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0).xy;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(vec2(projCoords.xy + vec2(x, y) * texelSize), index)).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{          
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos);
    vec2 texCoords;
    vec3 tangentViewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    if (displacementMapActive) {
        texCoords = ParallaxMapping(fs_in.TexCoords,  tangentViewDir);
        /*if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;*/
    } else {
        texCoords = fs_in.TexCoords;
    }


    vec4 tex = texture(diffuseMap, texCoords);
    if (tex.a < 0.1)
        discard;
    vec3 color = tex.rgb;


    vec3 normal = fs_in.Normal;
    if (normalMapActive) {
        normal = texture(normalMap, fs_in.TexCoords).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(fs_in.TBN * normal);
    }
    normal = normalize(normal); 
    //vec3 rgb_normal = normal * 0.5 + 0.5; // transforms from [-1,1] to [0,1]  

    vec3 result = vec3(0);
    // phase 1: directional lighting
    for(int i = 0; i < fs_in.dirLightsNum && i < DIR_LIGHTS_MAX; i++) {
        float shadow = ShadowCalculation(fs_in.FragPosDirLightSpace[i], normal, dirLights[i].position, dirLights[i].index);
        result += CalcDirLight(dirLights[i], normal, fs_in.FragPos, viewDir, shadow);
    }
    // phase 2: point lights
    for(int i = 0; i < fs_in.pointLightsNum && i < POINT_LIGHTS_MAX; i++) {
        float shadow = 0;
        for (int j = 0; j < 6; j++)
            shadow += ShadowCalculation(fs_in.FragPosPointLightSpace[i*6+j], normal, pointLights[i].position, pointLights[i].index+j);
        result += CalcPointLight(pointLights[i], normal, fs_in.FragPos, viewDir, shadow);    
    }
    // phase 3: spot light
    for(int i = 0; i < fs_in.spotLightsNum && i < SPOT_LIGHTS_MAX; i++) {
        float shadow = ShadowCalculation(fs_in.FragPosSpotLightSpace[i], normal, spotLights[i].position, spotLights[i].index);
        result += CalcSpotLight(spotLights[i], normal, fs_in.FragPos, viewDir, shadow);    
    }
    

    float gamma = 2.2;
    FragColor = vec4(result, 1.0) * tex;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return max(ambient + (1.0 - shadow) * (diffuse + specular), 0.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + max(light.quadratic, 0.032) * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return max(ambient + diffuse + specular, 0.0);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + max(light.linear, 0.032) * distance + max(light.quadratic, 0.032) * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return max(ambient + diffuse + specular, 0.0);
}