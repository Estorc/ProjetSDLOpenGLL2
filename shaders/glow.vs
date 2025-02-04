#version 330 core

layout(location = 0) in vec3 aPos;      // Vertex position
layout(location = 1) in vec3 aNormal;   // Vertex normal

uniform mat4 model;        // Model matrix
uniform mat4 view;         // View matrix
uniform mat4 projection;   // Projection matrix
uniform float outlineWidth;

void main() {
    // Extrude the vertex position along its normal
    vec3 extrudedPos = aPos + aNormal * outlineWidth;

    // Transform to clip space
    gl_Position = projection * view * model * vec4(extrudedPos, 1.0);
}