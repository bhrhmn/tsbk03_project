#version 150

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D posTex;
uniform sampler2D velTex;
uniform float delta_time;

void main(void)
{
	vec4 pos = texture(posTex, texCoord);
	vec4 vel = texture(velTex, texCoord);
	// leapfrog method
	vec4 gravity = vec4(0.0, -9.81, 0.0, 0.0);
	vec4 v = vel + gravity*delta_time;
	outColor = v / 256;



	// float down_speed = 1.0/256;
	// outColor = vec4(0.0, -down_speed*0.2, 0.0, 0.0);

	// outColor = vel;
}
