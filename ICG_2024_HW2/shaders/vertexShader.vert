#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float squeezeFactor;
uniform int useSqueeze;

void main() 
{
    // TODO: Implement squeeze effect
	//   1. Adjust the vertex position to create a squeeze effect based on squeezeFactor.
	//   2. Set gl_Position to be the multiplication of the perspective matrix (projection),
	//		view matrix (view), model matrix (model) and the adjusted vertex position.
	//   3. Set TexCoord to aTexCoord.
	// Note: Ensure to handle the squeeze effect for both y and z coordinates.
	vec3 squeezedPosition = aPos;
	if (useSqueeze == 1) {
		squeezedPosition.y += squeezedPosition.z * sin(squeezeFactor) * 0.5f;
		squeezedPosition.z += squeezedPosition.y * sin(squeezeFactor) * 0.5f;
	}
	gl_Position = projection * view * model * vec4(squeezedPosition, 1.0);
	TexCoord = aTexCoord;
}