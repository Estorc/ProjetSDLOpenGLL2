#include "../raptiquax.h"
#include "../math/math_util.h"
#include "../io/shader.h"
#include "render.h"
#include "depth_map.h"
#include "../settings.h"

void create_depthmap(DepthMap *depthMap, struct WorldShaders *shaders) {

    // Allocate enough space for your matrices
    size_t numDirectionalLights = NUM_DIRECTIONAL_LIGHTS;
    size_t numPointLights = NUM_POINT_LIGHTS*6;
    size_t numSpotLights = NUM_SPOT_LIGHTS;

    glGenBuffers(1, &depthMap->tbo);
    glBindBuffer(GL_TEXTURE_BUFFER, depthMap->tbo);

    size_t bufferSize = sizeof(mat4) * (numDirectionalLights + numPointLights + numSpotLights);
    glBufferData(GL_TEXTURE_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);

    glGenTextures(1, &depthMap->matrixTexture);
    glBindTexture(GL_TEXTURE_BUFFER, depthMap->matrixTexture);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, depthMap->tbo);

    glBindTexture(GL_TEXTURE_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);



    glGenFramebuffers(1, &depthMap->frameBuffer);  
    glGenTextures(1, &depthMap->texture);
    // Disable binding if settings do not allow shadows
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap->texture);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT16,
                Game.settings->shadow_resolution, 
                Game.settings->shadow_resolution,
                numDirectionalLights + numPointLights + numSpotLights);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);  
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);  

    use_shader(shaders->render);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap->texture);
    use_shader(0);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMap->frameBuffer);
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap->texture, 0, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}  