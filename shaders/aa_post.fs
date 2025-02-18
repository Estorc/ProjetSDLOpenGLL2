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
    vec3 col = texture(screenTexture,fragCoord).rgb;
    col = pow(col, vec3(1.0/1.2));  
    
    vec4 hud = texture(UITexture,vec2(fragCoord.x, -fragCoord.y));
    col = mix(col, hud.rgb, hud.a);
    
    fragColor = mix(vec4(col,1.0), fadeColor, fadeColor.a);
} 