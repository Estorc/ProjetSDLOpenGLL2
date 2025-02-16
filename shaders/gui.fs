#version 330 core
out vec4 fragColor;

in vec2 fragCoord;

uniform sampler2D background;
uniform sampler2D content;
uniform vec4 color;

uniform bool backgroundEnabled;
uniform bool contentEnabled;
uniform bool hovered;
uniform bool pressed;
uniform bool checked;
uniform bool isCheckBox;
uniform bool isRadioButton;

uniform vec4 overflow;
uniform vec2 pixelPosition;
uniform vec2 pixelSize;
uniform float time;

#define FRAME_RIGHT 168.0
#define FRAME_CENTER 120.0
#define FRAME_LEFT 96.0

#define FRAME_TOP 0.0
#define FRAME_MIDDLE -24.0
#define FRAME_BOTTOM -72.0

#define SCROLL_BAR_TOP -96.0
#define SCROLL_BAR_LEFT 0.0

#define PIXEL_SCALE 1.0
#define CHUNK_SIZE 24.0

#define WINDOWSKIN_SIZE 192.0

#define PIXEL_SCALE_X pixelSize.x / WINDOWSKIN_SIZE
#define PIXEL_SCALE_Y pixelSize.y / WINDOWSKIN_SIZE


void main()
{
    if (fragCoord.x*pixelSize.x + pixelPosition.x < overflow.x || fragCoord.y*pixelSize.y + pixelPosition.y < overflow.y || fragCoord.x*pixelSize.x + pixelPosition.x > overflow.z || fragCoord.y*pixelSize.y + pixelPosition.y > overflow.w) {
        discard;
    }
    vec4 backgroundColor = vec4(0.0);
    vec4 contentColor = vec4(0.0);
    vec2 pixelFragCoord = vec2(fragCoord.x*(WINDOWSKIN_SIZE), fragCoord.y*(WINDOWSKIN_SIZE));
    vec2 pixelCoord = fragCoord*pixelSize;
    vec2 layerCoord;
    layerCoord.x = (pixelCoord.x / (CHUNK_SIZE*PIXEL_SCALE_X/6.0) + FRAME_CENTER)/pixelSize.x * PIXEL_SCALE_X;
    layerCoord.y = (-pixelCoord.y / (CHUNK_SIZE*PIXEL_SCALE_Y/6.0) + FRAME_MIDDLE)/pixelSize.y * PIXEL_SCALE_Y;
    if (isCheckBox) {
        backgroundColor = texture(background, vec2((fragCoord.x)/8.1+6.0/8.0, -(fragCoord.y)/8.1-6.0/8.0));
        if (hovered)
            backgroundColor = mix(backgroundColor, texture(background, vec2((fragCoord.x)/8.1+7.0/8.0, -(fragCoord.y)/8.1-6.0/8.0)), abs(sin(time/250.0))/1.5);
        if (pressed || checked)
            backgroundColor = texture(background, vec2((fragCoord.x)/8.1+7.0/8.0, -(fragCoord.y)/8.1-6.0/8.0));
    } else if (isRadioButton) {
        backgroundColor = texture(background, vec2((fragCoord.x)/8.1+4.0/8.0, -(fragCoord.y)/8.1-6.0/8.0));
        if (hovered)
            backgroundColor = mix(backgroundColor, texture(background, vec2((fragCoord.x)/8.1+5.0/8.0, -(fragCoord.y)/8.1-6.0/8.0)), abs(sin(time/250.0))/1.5);
        if (pressed || checked)
            backgroundColor = texture(background, vec2((fragCoord.x)/8.1+5.0/8.0, -(fragCoord.y)/8.1-6.0/8.0));
    } else {
        if (hovered)
            backgroundColor = texture(background, vec2((fragCoord.x)/4.1+0.5, -(fragCoord.y)/4.1+0.501)) * abs(sin(time/250.0))/1.5;
        if (pressed || checked)
            backgroundColor = texture(background, vec2((fragCoord.x)/4.1+0.5, -(fragCoord.y)/4.1+0.501));
        if (backgroundEnabled) {
            //vec4 bgLayerColor = texture(background, vec2(mod((fragCoord.x*pixelSize.x)/400.0, 0.5), mod((-fragCoord.y*pixelSize.y)/400.0, 0.5)));
            vec4 bgLayerColor;
            if (pixelCoord.x < CHUNK_SIZE/PIXEL_SCALE)
                layerCoord.x = (mod(pixelCoord.x, CHUNK_SIZE) + FRAME_LEFT)/pixelSize.x * PIXEL_SCALE_X;
            if (pixelCoord.y < CHUNK_SIZE/PIXEL_SCALE)
                layerCoord.y = (-mod(pixelCoord.y, CHUNK_SIZE) + FRAME_TOP)/pixelSize.y * PIXEL_SCALE_Y;
            if (pixelCoord.x + CHUNK_SIZE/PIXEL_SCALE > pixelSize.x)
                layerCoord.x = (mod(pixelCoord.x + mod(CHUNK_SIZE - pixelSize.x, CHUNK_SIZE), CHUNK_SIZE) + FRAME_RIGHT)/pixelSize.x * PIXEL_SCALE_X;
            if (pixelCoord.y + CHUNK_SIZE/PIXEL_SCALE > pixelSize.y)
                layerCoord.y = (-mod(pixelCoord.y + mod(CHUNK_SIZE - pixelSize.y, CHUNK_SIZE), CHUNK_SIZE) + FRAME_BOTTOM)/pixelSize.y * PIXEL_SCALE_Y;
            bgLayerColor = texture(background, layerCoord);

            if (backgroundEnabled && backgroundColor.a < 0.1) backgroundColor = texture(background, vec2(layerCoord.x-0.5, layerCoord.y));
            backgroundColor = mix(backgroundColor, bgLayerColor, bgLayerColor.a);
        }
    }

    if (contentEnabled)
        contentColor = texture(content, fragCoord);
    vec4 premultipliedContent = vec4(contentColor.rgb * contentColor.a, contentColor.a);
    vec4 premultipliedBackground = vec4(backgroundColor.rgb * (1.0 - contentColor.a), backgroundColor.a);
    fragColor = premultipliedContent + premultipliedBackground;
    //fragColor = vec4(fragCoord.x, fragCoord.y, 0.0, 1.0);
} 