#version 330 core

// TODO 2
// Implement Bling-Phong shading

out vec4 FragColor;

in vec2 TexCoord;
in vec3 fragPos;
in vec3 fragNormal;

uniform sampler2D ourTexture;
uniform samplerCube envTexture;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightAmb;
uniform vec3 lightDiff;
uniform vec3 lightSpec;
uniform vec3 matAmb;
uniform vec3 matDiff;
uniform vec3 matSpec;
uniform float matGloss;

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 light = normalize(lightPos - fragPos);
    vec3 view = normalize(cameraPos - fragPos);

    // calculate reflection
    vec3 reflection = (-view) - (2 * dot(-view, normal) * normal);
    vec3 envColor = texture(envTexture, reflection).rgb;
    vec3 modelColor = texture(ourTexture, TexCoord).rgb;
    
    // calculate B and fragColor
    float intensity = 1.0;
    float bias = 0.2;
    float alpha = 0.4;
    float B = max(dot(light, normal) * intensity, 0.0) + bias;
    FragColor = vec4((alpha * B * modelColor) + ((1 - alpha) * envColor), 1.0);
}