#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

out vec2 TexCoords;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool haveBone;

out VS_OUT {
    vec4 FragGPos;
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 viewPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    mat3 TBN;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 viewPos;

void main()
{

    vec4 tPos = vec4(0.0);
    vec3 tNormal = aNormal;
    if (haveBone) {
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(boneIds[i] == -1) 
                continue;
            if(boneIds[i] >=MAX_BONES) 
            {
                tPos = vec4(aPos,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
            tPos += localPosition * weights[i];
            vec3 tNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
        }
    } else tPos = vec4(aPos,1.0);

    gl_Position = projection * view * model * tPos;
    vs_out.FragGPos = view * model * tPos;
    vs_out.FragPos = vec3(model * tPos);
    vs_out.Normal = transpose(inverse(mat3(model))) * tNormal;
    vs_out.TexCoords = aTexCoords;

    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(transpose(inverse(mat3(model))) * tNormal);
    vs_out.TBN = transpose(mat3(T, B, N));
    vs_out.viewPos = viewPos;
    vs_out.TangentViewPos  = vs_out.TBN * viewPos;
    vs_out.TangentFragPos  = vs_out.TBN * vs_out.FragPos;
}