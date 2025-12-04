#version 150

out vec4 out_Color;
uniform sampler2D tex;
uniform sampler2D pos1;
in vec2 texCoord;

void main(void)
{
	vec4 t = texture(pos1, texCoord);
	if (t.x < 0.05 && t.y < 0.05 && t.z < 0.05) 
		discard;
	else
		out_Color = t;

	// out_Color = texture(pos1, texCoord);
}
