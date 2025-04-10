#version 150
uniform sampler2D texUnit; 

uniform vec3 firePos;
uniform vec3 fireColor;

out vec4 outColor;
uniform mat4 myMatrix;
in vec3 exColor;
in vec3 transformedNormal;
in vec2 outTexCord;


void main(void)
{
	/* do light here */ 
	outColor = vec4(fireColor, 1.0) * texture(texUnit, outTexCord);
}
