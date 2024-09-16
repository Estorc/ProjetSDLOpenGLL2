#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 camPos;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos + camPos, 1.0);
    gl_Position = pos.xyww;
}  