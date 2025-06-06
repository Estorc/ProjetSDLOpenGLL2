#version 430 core
// https://imanolfotia.com/blog/1

uniform sampler2D gFinalImage;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gExtraComponents;
uniform sampler2D ColorBuffer; 

uniform mat4 projection;
uniform mat4 view;
uniform mat4 invView;
uniform mat4 invprojection;

in vec2 TexCoords;

out vec4 outColor;

const float step = 0.1;
const float minRayStep = 0.1;
const float maxSteps = 30;
const int numBinarySearchSteps = 5;
const float reflectionSpecularFalloffExponent = 3.0;

float Metallic;

#define Scale vec3(.8, .8, .8)
#define K 19.19

vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
vec4 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 hash(vec3 a);
vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth);

void main()
{
    vec2 MetallicEmmissive = texture(gExtraComponents, TexCoords).rg;
    Metallic = MetallicEmmissive.r;
    vec3 albedo = texture(gFinalImage, TexCoords).rgb;

    if(Metallic < 0.01) {
        outColor = vec4(albedo,1.0);
        return;
    }
 
    vec3 viewNormal = vec3(texture(gNormal, TexCoords) * invView);
    vec3 viewPos = textureLod(gPosition, TexCoords, 2).xyz;

    float spec = texture(ColorBuffer, TexCoords).w;

    vec3 F0 = vec3(0.04); 
    F0      = mix(F0, albedo, Metallic);
    vec3 Fresnel = fresnelSchlick(max(dot(normalize(viewNormal), normalize(viewPos)), 0.0), F0);

    // Reflection vector
    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));


    vec3 hitPos = viewPos;
    float dDepth;
 
    vec3 wp = vec3(vec4(viewPos, 1.0) * invView);
    vec3 jitt = mix(vec3(0.0), vec3(hash(wp)), spec);
    vec4 coords = RayMarch((vec3(jitt) + reflected * max(minRayStep, -viewPos.z)), hitPos, dDepth);
 
 
    vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - coords.xy));
 
 
    float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

    float ReflectionMultiplier = pow(Metallic, reflectionSpecularFalloffExponent) * 
                screenEdgefactor * 
                -reflected.z;
 
    // Get color
    vec3 SSR = textureLod(gFinalImage, coords.xy, 0).rgb * clamp(ReflectionMultiplier, 0.0, 0.9) * Fresnel;  

    outColor = vec4(albedo, 1.0) + vec4(SSR, Metallic);
    //outColor = vec4(vec3(viewPos), 1.0);  // Scale to fit in [0, 1] range
}

vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth)
{
    float depth;

    vec4 projectedCoord;
 
    for(int i = 0; i < numBinarySearchSteps; i++)
    {

        projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(gPosition, projectedCoord.xy, 2).z;

 
        dDepth = hitCoord.z - depth;

        dir *= 0.5;
        if(dDepth > 0.0)
            hitCoord += dir;
        else
            hitCoord -= dir;    
    }

        projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
    return vec3(projectedCoord.xy, depth);
}

vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{

    dir *= step;
 
 
    float depth;
    int steps;
    vec4 projectedCoord;

 
    for(int i = 0; i < maxSteps; i++)
    {
        hitCoord += dir;
 
        projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(gPosition, projectedCoord.xy, 2).z;
        if(depth > 1000.0)
            continue;
 
        dDepth = hitCoord.z - depth;

        if((dir.z - dDepth) < 1.2)
        {
            if(dDepth <= 0.0)
            {   
                vec4 Result;
                Result = vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);

                return Result;
            }
        }
        
        steps++;
    }
 
    
    return vec4(projectedCoord.xy, depth, 0.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}



vec3 hash(vec3 a)
{
    a = fract(a * Scale);
    a += dot(a, a.yxz + K);
    return fract((a.xxy + a.yxx)*a.zyx);
}