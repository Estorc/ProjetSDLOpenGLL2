#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 fragCoord;
uniform mat4 model;

void main()
{
    fragCoord = aTexCoords;
    gl_Position = model * vec4(-floor(aPos.x), floor(aPos.y), 0.0, 1.0); 
}  