#version 330 core

// TODO 6-2
// Implement Glass-Empricial shading


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

    // calculate reflect
    vec3 reflection = (-view) - (2 * dot(-view, normal) * normal);
    vec3 reflectColor = texture(envTexture, reflection).rgb;

    // calcuate refraction based on Snell's law
    float n1 = 1.0;
    float n2 = 1.52;
    float eta = n1 / n2;  // airRef / objRef
    float cosTheta = dot(-view, normal);
    if (cosTheta > 0.0) {
        eta = n2 / n1;
    }
    float k = 1.0 - eta * eta * (1.0 - cosTheta * cosTheta);
    vec3 refracted = vec3(0, 0, 0);
    if (k >= 0.0){
        refracted = eta * (-view) - (eta * cosTheta + sqrt(k)) * normal;
    }
    vec3 refraColor = texture(envTexture, refracted).rgb;

    // calculate ratio
    float scale = 0.7;
    float power = 2.0;
    float bias = 0.2;
    float R = max(0.0, min(1, bias + scale * pow((1 + cosTheta), power)));
    FragColor = vec4(R * reflectColor + (1 - R) * refraColor, 1.0);
}