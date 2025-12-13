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
	
	// if ((abs(vel.y) < 0.005) && (pos.y < 0))
	// {
	// 	// Start over, randomize speed
	// 	pos.x = texCoord.x;
	// 	pos.y = 0;
	// 	pos.z = texCoord.y;
	// 	outColor = pos;
	// }
	// else
	outColor = pos + vel;

	// outColor = pos + vec4(0.0, -0.0001, 0.0, 0.0);
	// outColor = vec4(0.0, 1.0, 0.0, 1.0);

	// outColor = pos;
}
