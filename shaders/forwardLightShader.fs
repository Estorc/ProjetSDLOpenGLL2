#version 430 core

layout(binding = 0, rgba32f) uniform image2D lightMap;
layout(binding = 1, rgba32f) uniform image2D intermediateLightMap;

void main()
{
    vec4 lightMean = imageLoad(lightMap, ivec2(gl_FragCoord.xy));
    vec3 Lo = imageLoad(intermediateLightMap, ivec2(gl_FragCoord.xy)).rgb;
    float smoothingFactor = 1.0/10.0;

    lightMean = vec4(mix(lightMean.rgb, Lo, smoothingFactor), 1.0);
    imageStore(lightMap, ivec2(gl_FragCoord.xy), lightMean);
} 