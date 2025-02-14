#version 330 core
out vec4 FragColor;

in vec2 TexCoord; 

uniform sampler2D ourTexture;
uniform vec3 rainbowColor;
uniform int useRainbowColor;
uniform int useHeli;

void main()
{
    // TODO: Implement Rainbow Effect
	//   1. Retrieve the color from the texture at texCoord.
	//   2. Set FragColor to be the dot product of the color and rainbowColor
	//   Note: Ensure FragColor is appropriately set for both rainbow and normal cases.
	vec4 texColor = texture(ourTexture, TexCoord);
	if (useRainbowColor == 1) {
		FragColor = vec4(texColor.rgb * rainbowColor, texColor.a);
	}
	else if (useHeli == 1) {
		// vec3 lightPos = vec3(0, 200, 100);
		// vec3 lightColor = vec3(1,1,1);
		FragColor = vec4(rainbowColor, 1);
	}
	else {
		FragColor = texColor;
	}
}