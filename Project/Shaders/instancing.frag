#version 150

out vec4 out_Color;
uniform sampler2D tex;
in vec2 texCoord;

void main(void)
{
	vec4 t = texture(tex, texCoord);
	if (t.x < 0.05 && t.y < 0.05 && t.z < 0.05) 
		discard;
	else
		out_Color = t;
}
