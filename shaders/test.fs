#version 430 core
uniform sampler2DArrayShadow shadowMap;

in vec4 fragPosLightSpace;

void main() {
    // Perspective divide and transform to [0, 1] range
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // Map to [0, 1]

    // Sample the shadow map (automatic depth comparison)
    float shadow = texture(shadowMap, vec4(projCoords,1.0));

    gl_FragColor = vec4(shadow); // Example of just rendering the shadow
}