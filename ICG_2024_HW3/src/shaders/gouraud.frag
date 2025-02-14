#version 330 core

// TODO 3

out vec4 FragColor;

in vec2 TexCoord;
in vec3 ambient;
in vec3 diffuse;
in vec3 specular;

uniform sampler2D ourTexture;



void main() {
    vec3 color = texture(ourTexture, TexCoord).rgb;

    vec3 amb = ambient * color;

    vec3 diff = diffuse * color;    

    FragColor = vec4((amb + diff + specular), 1.0);
}