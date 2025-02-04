#version 330 core

in vec2 TexCoords;
uniform sampler2D diffuseMap;

void main()
{   
    if (texture(diffuseMap, TexCoords).a < 0.1) discard;        
    //gl_FragDepth = gl_FragCoord.z;
}  