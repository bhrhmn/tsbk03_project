#version 150

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D posTex;
uniform sampler2D velTex;
uniform float time;

void main(void)
{
	vec4 pos = texture(posTex, texCoord);
	vec4 vel = texture(velTex, texCoord);
	
	if (length(vec3(pos)) < 0.001) // restart!
	{
		vel.x = 0.02*(fract((time*0.11 + texCoord.s*17 - texCoord.t*11) * 123)-0.5);
		vel.z = 0.02*(fract((time*0.12 + texCoord.s*9 + texCoord.t*13) * 219)-0.5);
		vel.y = 0.03 + sin(time + texCoord.s + texCoord.t)/60.0; // Create pumping effect
	}
	
	if (pos.y < 0)
		vel.y = 0.8*abs(vel.y);
	vel.y -= 0.001;

	// // test
	// outColor = vec4(1.0, 1.0, 0.0, 1.0);

	outColor = vel;
}
