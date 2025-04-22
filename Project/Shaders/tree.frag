#version 150
uniform sampler2D texUnit; 

uniform vec3 firePos;
uniform vec3 fireColor;

out vec4 outColor;
in vec3 transformedNormal;
in vec2 outTexCord;


void main(void)
{
	vec4 t = texture(texUnit, outTexCord);
	if (t.x < 0.01 && t.y < 0.01 && t.z < 0.01) discard;
	else {
		outColor = t*0.8;
	}
}