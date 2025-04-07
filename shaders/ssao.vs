#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 noiseScale; 

uniform float screenWidth;
uniform float screenHeight;

void main()
{
    TexCoords = aTexCoords;
    noiseScale = vec2(screenWidth, screenHeight) / 4.0; // 4x4 noise texture
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  