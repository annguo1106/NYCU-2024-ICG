#version 330 core

// TODO 3

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 ambient;
out vec3 diffuse;
out vec3 specular;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
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
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;

    vec3 normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    vec4 worldPos = model * vec4(aPos, 1.0);

    ambient = lightAmb * matAmb;

    vec3 light = normalize(lightPos - vec3(worldPos));
    diffuse = lightDiff * matDiff * dot(light, normal);

    vec3 viewVec = normalize(cameraPos - vec3(worldPos));
    vec3 reflect = normalize(reflect(-light, normal));
    specular = lightSpec * matSpec * pow(max(0.0, dot(viewVec, reflect)), matGloss);
}