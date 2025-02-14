#version 330 core

// TODO 2
// Implement Bling-Phong shading

out vec4 FragColor;

in vec2 TexCoord;
in vec3 fragPos;
in vec3 fragNormal;

uniform sampler2D ourTexture;
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
    vec3 halfVec = normalize((light + view));
    vec3 color = texture(ourTexture, TexCoord).rgb;

    vec3 ambient = lightAmb * matAmb * color;

    vec3 diffuse = lightDiff * matDiff * dot(light, normal) * color;

    vec3 specular = lightSpec * matSpec * pow(max(0.0, dot(normal, halfVec)), matGloss);

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}