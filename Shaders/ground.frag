#version 150
uniform sampler2D texUnit; 

uniform vec3 firePos;
uniform vec3 fireColor;

out vec4 outColor;
in vec3 transformedNormal;
in vec2 outTexCord;


void main(void)
{
	outColor =  texture(texUnit, outTexCord);

}