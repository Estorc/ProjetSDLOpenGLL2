#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FragPos;
layout (location = 2) out vec4 FragNormal;
layout (location = 3) out vec4 FragExtra;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
    FragPos = vec4(0.0, 0.0, 0.0, 1.0);
    FragNormal = vec4(0.0, 0.0, 0.0, 1.0);
    FragExtra = vec4(0.0, 0.0, 0.0, 1.0);
}