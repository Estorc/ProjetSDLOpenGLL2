#include "../types.h"
#include "../math/math_util.h"
#include "../io/shader.h"
#include "render.h"
#include "depth_map.h"
#include "../settings.h"

void create_depthmap(DepthMap *depthMap, struct WorldShaders *shaders) {

    // Allocate enough space for your matrices
    size_t numDirectionalLights = 100;
    size_t numPointLights = 100;
    size_t numSpotLights = 100;

    glGenBuffers(1, &depthMap->ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, depthMap->ubo);

    size_t bufferSize = sizeof(mat4) * (numDirectionalLights + numPointLights + numSpotLights);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);

    GLint bindingPoint = 0;
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, depthMap->ubo);

    GLuint blockIndex = glGetUniformBlockIndex(shaders->render, "LightMatrices");
    glUniformBlockBinding(shaders->render, blockIndex, bindingPoint);


    GLint actualBinding;
    glGetActiveUniformBlockiv(shaders->render, blockIndex, GL_UNIFORM_BLOCK_BINDING, &actualBinding);

    if (actualBinding != bindingPoint) {
        printf("Error: Uniform block binding point mismatch!");
    }



    glGenFramebuffers(1, &depthMap->frameBuffer);  
    glGenTextures(1, &depthMap->texture);
    // Disable binding if settings do not allow shadows
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap->texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, 
                SHADOW_WIDTH, SHADOW_HEIGHT, MAX_SHADOW, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);  
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);  

    glBindFramebuffer(GL_FRAMEBUFFER, depthMap->frameBuffer);
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap->texture, 0, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}  