#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 texelSize; 

uniform float screenWidth;
uniform float screenHeight;

void main()
{
    TexCoords = aTexCoords;
    texelSize = vec2(1.0 / screenWidth, 1.0 / screenHeight); // 1.0 / texture size
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  