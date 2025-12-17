#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D posTex;
uniform sampler2D velTex;
uniform sampler2D startPosTex;

void main(void)
{
	vec4 pos = texture(posTex, texCoord);
	vec4 vel = texture(velTex, texCoord);

	if (pos.y < -15.0/256.0) 
	{
		vec4 start_pos = texture(startPosTex, texCoord);
		pos.y = start_pos.y;
	}
	outColor = pos + vel;
}
