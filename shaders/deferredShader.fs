#version 430 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FragPos;
layout (location = 2) out vec4 FragNormal;
layout (location = 3) out vec4 FragExtra;

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

in VS_OUT {
    vec4 Pos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    mat3 TBN;
} fs_in;

uniform bool diffuseMapActive;
uniform bool normalMapActive;
uniform bool parallaxMapActive;
uniform bool metallicMapActive;
uniform bool roughnessMapActive;
uniform Material material;

float metallic = 0.0;
float roughness = 0.5;
vec3 albedo;
const float ao = 0.1;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    float heightScale = material.parallax;
    if (heightScale <= 0.0) return texCoords;
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

void main() {
    vec2 texCoords;
    vec3 tangentViewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    texCoords = parallaxMapActive ? ParallaxMapping(fs_in.TexCoords, tangentViewDir) : fs_in.TexCoords;
    if (parallaxMapActive && (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0))
        discard;

    metallic = metallicMapActive ? texture(metallicMap, texCoords).r : material.metallic;
    roughness = roughnessMapActive ? texture(roughnessMap, texCoords).r : material.roughness;

    vec4 tex = diffuseMapActive ? texture(diffuseMap, texCoords) : vec4(material.diffuse, 1.0);
    if (diffuseMapActive && tex.a < 0.1)
        discard;
    albedo = tex.rgb;
    vec3 normal = fs_in.Normal;
    if (normalMapActive) {
        normal = texture(normalMap, texCoords).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(fs_in.TBN * normal);
    }
    normal = normalize(normal); 

    FragColor = vec4(albedo, 1.0);
    FragPos = fs_in.Pos;
    FragNormal = vec4(normal, 1.0);
    FragExtra = vec4(metallic, roughness, material.shininess, 0.0);
}