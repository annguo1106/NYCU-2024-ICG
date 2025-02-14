#version 330 core

// TODO 6-1:
// Implement Glass-Schlick shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoord = aTexCoord;
    vec4 worldPos = model * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;
    fragNormal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * worldPos;        
}