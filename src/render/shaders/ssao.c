#include <raptiquax.h>
#include <utils/random.h>
#include <utils/interp.h>
#include <io/shader.h>

static vec3 ssaoKernel[64];
static vec3 ssaoNoise[16];

void init_ssao(Shader shaderSSAO, Shader shaderSSAOBlur) {
    // generate kernel samples
    // -----------------------
    for (unsigned int i = 0; i < 64; ++i)
    {
        vec3 sample = {random_float(0.0f, 1.0f) * 2.0f - 1.0f, random_float(0.0f, 1.0f) * 2.0f - 1.0f, random_float(0.0f, 1.0f)};
        glm_vec3_normalize(sample);
        glm_vec3_scale(sample, random_float(0.0f, 1.0f), sample);
        f32 scale = (f32) i / 64.0f;
    
        // scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        glm_vec3_scale(sample, scale, sample);
        glm_vec3_copy(sample, ssaoKernel[i]);
    }
    
    // generate noise texture
    // ----------------------
    for (unsigned int i = 0; i < 16; i++)
    {
        vec3 noise = {random_float(0.0f, 1.0f) * 2.0f - 1.0f, random_float(0.0f, 1.0f) * 2.0f - 1.0f, 0.0f}; // rotate around z-axis (in tangent space)
        glm_vec3_copy(noise, ssaoNoise[i]);
    }
    unsigned int noiseTexture; glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    use_shader(shaderSSAO);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    set_shader_int(shaderSSAO, "gPosition", 0);
    set_shader_int(shaderSSAO, "gNormal", 1);
    set_shader_int(shaderSSAO, "texNoise", 2);
    set_shader_int(shaderSSAO, "gExtraComponents", 3);
    for (unsigned int i = 0; i < 64; ++i) {
        char uniformName[64];
        sprintf(uniformName, "samples[%d]", i);
        set_shader_vec3(shaderSSAO, uniformName, ssaoKernel[i]);
    }
    use_shader(shaderSSAOBlur);
    set_shader_int(shaderSSAOBlur, "ssaoInput", 0);
    use_shader(0);
}