#include <raptiquax.h>
#include <math/math_util.h>
#include <io/shader.h>
#include <render.h>
#include <depth_map.h>
#include <settings.h>

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


    glGenTextures(1, &depthMap->texture);
    // Disable binding if settings do not allow shadows
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap->texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16,
        Game.settings->shadow_resolution, Game.settings->shadow_resolution,
        numDirectionalLights + numPointLights + numSpotLights, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);  
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);  

    use_shader(shaders->light);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap->texture);
    set_shader_int(shaders->light, "shadowMap", 8);
    use_shader(0);

    for (int i = 0; i < numDirectionalLights + numPointLights + numSpotLights; i++) {
        glGenFramebuffers(1, &depthMap->frameBuffer[i]);  
        use_fbo(GL_FRAMEBUFFER, depthMap->frameBuffer[i]);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap->texture, 0, i);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    use_fbo(GL_FRAMEBUFFER, 0); 
}  