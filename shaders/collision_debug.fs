#version 330 core
out vec4 FragColor;
in vec2 TexCoords;


void main()
{          
    if (TexCoords.x > 0.05 && TexCoords.x < 0.95 && TexCoords.y > 0.05 && TexCoords.y < 0.95) {
        discard;
    }
    FragColor = vec4(0.3, 1.0, 0.6, 1.0);
}
