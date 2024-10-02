#version 330 core
out vec4 fragColor;

#define PI 3.14159265358979323846264338327

in vec2 fragCoord;

uniform sampler2D screenTexture;
uniform sampler2D UITexture;
uniform float iTime;
uniform vec2 iResolution;




void main()
{
    /*
    // parameters
    float wave = 10.0;
    float velocity1 = 1.0;
    float raio = 1.0;
    // parameters
    
    vec2 uv = fragCoord/iResolution.xy;
    
    
    float dist = distance(uv,vec2(0.5,0.5));
    
    float ripple = cos(sin(dist*wave-iTime*velocity1));
    
    vec3 col;
    
    if(dist<raio)
    	col = texture(screenTexture,uv+ripple).rgb;
    else
        col = texture(screenTexture,uv).rgb;
    */
    vec3 col = texture(screenTexture,fragCoord).rgb;
    //col *= 1-(pow((fragCoord.x-0.5)*2.0,2) + pow((fragCoord.y-0.5)*1.5,2));
    //col *= min(0.2/(pow((fragCoord.x-0.5),2) + pow((fragCoord.y-0.5),2)), 2.0);
    vec4 hud = texture(UITexture,vec2(fragCoord.x, -fragCoord.y));
    col = mix(col, hud.rgb, hud.a);
    
    fragColor = vec4(col,1.0);
} 