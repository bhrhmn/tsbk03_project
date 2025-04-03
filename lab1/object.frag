#version 150
uniform sampler2D texUnit; 


out vec4 outColor;
uniform mat4 myMatrix;
in vec3 exColor;
in vec3 transformedNormal;
in vec2 outTexCord;


void main(void)
{
	outColor =  vec4(0.0, 0.0, 1.0, 1.0);
}
