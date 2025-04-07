#version 430 core

uniform sampler2D gFinalImage;
const float threshold = 0.5;  // Seuil pour extraire les zones lumineuses
const float intensity = 3.5;  // Intensité du bloom

in vec2 texelSize;
in vec2 TexCoords;
out vec4 FragColor;

// Poids du noyau Gaussien 9×9
const float kernel[9] = float[](0.05, 0.09, 0.12, 0.15, 0.18, 0.15, 0.12, 0.09, 0.05);

// Luminance (perception de la luminosité)
float Luma(vec3 color) {
    return dot(color, vec3(0.299, 0.587, 0.114));
}

// Extrait les parties lumineuses uniquement
vec3 ExtractBrightParts(vec3 color) {
    return max(color - vec3(threshold), 0.0);
}

// Applique un flou Gaussien 9x9 dans les deux directions
vec3 GaussianBlur(vec2 coords) {
    vec3 bloomColor = vec3(0.0);
    float totalWeight = 0.0;

    for (int x = -4; x <= 4; x++) {
        for (int y = -4; y <= 4; y++) {
            vec2 offset = vec2(float(x) * texelSize.x, float(y) * texelSize.y);
            vec3 color = texture(gFinalImage, coords + offset).rgb;
            vec3 bright = ExtractBrightParts(color);  // Appliquer le flou uniquement aux parties brillantes
            float weight = kernel[abs(x)] * kernel[abs(y)];
            bloomColor += bright * weight;
            totalWeight += weight;
        }
    }

    return bloomColor / totalWeight; // Normalisation
}

void main() {
    // Chargement de la couleur originale
    vec3 color = texture(gFinalImage, TexCoords).rgb;

    // Appliquer un flou Gaussien uniquement aux zones lumineuses
    vec3 blurredGlow = GaussianBlur(TexCoords) * intensity;

    // Additionner la lueur à l'image d'origine
    vec3 finalColor = color + blurredGlow;

    FragColor = vec4(finalColor, 1.0);
}
