#version 150
uniform sampler2D texUnit; 

uniform vec3 firePos;
uniform vec3 fireColor;

out vec4 outColor;
in vec3 transformedNormal;
in vec2 outTexCord;

uniform float shade;

void main(void)
{
	vec4 t = texture(texUnit, outTexCord);
	if (t.x < 0.05 && t.y < 0.05 && t.z < 0.05) discard;
	else if (t.x > 0.80 && t.y > 0.80 && t.z > 0.80) discard;
	else {
		outColor = t*shade;
	}
}