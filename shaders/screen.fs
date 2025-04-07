#version 330 core
out vec4 fragColor;

#define PI 3.14159265358979323846264338327

in vec2 fragCoord;

uniform sampler2D screenTexture;
uniform sampler2D UITexture;
uniform float iTime;
uniform vec4 fadeColor;
uniform vec2 iResolution;




void main()
{
    vec4 texColor = texture(screenTexture, fragCoord);
    vec4 hud = texture(UITexture,fragCoord);
    texColor = mix(texColor, hud, hud.a);
    fragColor = mix(texColor, fadeColor, fadeColor.a);
} 