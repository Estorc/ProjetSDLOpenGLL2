#version 430 core
uniform sampler2D gFinalImage;

in vec2 texelSize; // (1.0 / screenWidth, 1.0 / screenHeight)
in vec2 TexCoords;
out vec4 FragColor; // Stores detected edges

float Luma(vec3 color) {
    return dot(color, vec3(0.299, 0.587, 0.114));
}

float ComputeEdgeContrast(vec2 coords) {
    float center = Luma(texture(gFinalImage, coords).rgb);

    float left   = Luma(texture(gFinalImage, coords + vec2(-texelSize.x,  0.0)).rgb);
    float right  = Luma(texture(gFinalImage, coords + vec2( texelSize.x,  0.0)).rgb);
    float top    = Luma(texture(gFinalImage, coords + vec2( 0.0,  texelSize.y)).rgb);
    float bottom = Luma(texture(gFinalImage, coords + vec2( 0.0, -texelSize.y)).rgb);

    float topLeft     = Luma(texture(gFinalImage, coords + vec2(-texelSize.x,  texelSize.y)).rgb);
    float topRight    = Luma(texture(gFinalImage, coords + vec2( texelSize.x,  texelSize.y)).rgb);
    float bottomLeft  = Luma(texture(gFinalImage, coords + vec2(-texelSize.x, -texelSize.y)).rgb);
    float bottomRight = Luma(texture(gFinalImage, coords + vec2( texelSize.x, -texelSize.y)).rgb);

    float gradientH = abs(left - right) + abs(topLeft - topRight) + abs(bottomLeft - bottomRight);
    float gradientV = abs(top - bottom) + abs(topLeft - bottomLeft) + abs(topRight - bottomRight);

    return max(gradientH, gradientV);
}

float ComputeBlendFactor(float edgeContrast) {
    return smoothstep(0.02, 0.2, edgeContrast);
}

vec3 SubPixelBlend(vec2 coords, float blendFactor) {
    vec3 color = texture(gFinalImage, coords).rgb;

    vec3 leftColor  = texture(gFinalImage, coords + vec2(-texelSize.x,  0.0)).rgb;
    vec3 rightColor = texture(gFinalImage, coords + vec2( texelSize.x,  0.0)).rgb;
    vec3 topColor   = texture(gFinalImage, coords + vec2( 0.0,  texelSize.y)).rgb;
    vec3 bottomColor= texture(gFinalImage, coords + vec2( 0.0, -texelSize.y)).rgb;

    vec3 topLeft     = texture(gFinalImage, coords + vec2(-texelSize.x,  texelSize.y)).rgb;
    vec3 topRight    = texture(gFinalImage, coords + vec2( texelSize.x,  texelSize.y)).rgb;
    vec3 bottomLeft  = texture(gFinalImage, coords + vec2(-texelSize.x, -texelSize.y)).rgb;
    vec3 bottomRight = texture(gFinalImage, coords + vec2( texelSize.x, -texelSize.y)).rgb;

    vec3 avgBlend = (leftColor + rightColor + topColor + bottomColor + 
                     topLeft + topRight + bottomLeft + bottomRight) * 0.125;

    return mix(color, avgBlend, blendFactor);
}

void main() {
    float edgeContrast = ComputeEdgeContrast(TexCoords);
    float blendFactor = ComputeBlendFactor(edgeContrast);
    FragColor = vec4(SubPixelBlend(TexCoords, blendFactor), 1.0);
}