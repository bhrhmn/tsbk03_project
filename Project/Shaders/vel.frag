#version 150

out vec4 outColor;

in vec2 texCoord;
uniform sampler2D velTex;
uniform float delta_time;

void main(void)
{
	vec4 vel = texture(velTex, texCoord);
	// update velocity
	vec4 gravity = vec4(0.0, -9.81, 0.0, 0.0);
	vec4 v = vel + gravity*delta_time;
	outColor = v / 256.0;
}
