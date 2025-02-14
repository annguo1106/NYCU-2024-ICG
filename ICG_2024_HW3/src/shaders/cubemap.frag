#version 330 core

// TODO 4-1
// Implement CubeMap shading
out vec4 FragColor;

in vec3 TexCoord; 

uniform samplerCube ourTexture;

void main() {
    FragColor = texture(ourTexture, TexCoord);
}