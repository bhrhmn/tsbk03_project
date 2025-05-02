#version 150

// Simple texture shader
// Texture coordinates from vertex shader

in vec2 texcoord;
out vec4 outColor;
uniform sampler2D tex;
in vec3 exNormal;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);

	vec4 t = texture(tex, texcoord).rrrr;
	float shade = dot(exNormal, light);
	outColor = shade * t;
//	outColor = texture(tex, texcoord).rrrr;
}
