#version 330 core

// TODO 4-1
// Implement CubeMap shading
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
	TexCoord = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}