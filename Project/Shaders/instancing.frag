#version 150

out vec4 out_Color;
uniform sampler2D tex;
uniform sampler2D pos1;
uniform sampler2D pos2;
in vec2 texCoord;

void main(void)
{
	vec4 t = texture(tex, texCoord);
	vec4 p1 = texture(pos1, texCoord);
	vec4 p2 = texture(pos2, texCoord);
	// if (t.x < 0.05 && t.y < 0.05 && t.z < 0.05) 
	// 	discard;
	// else
		out_Color = t;

	// out_Color = texture(pos1, texCoord);
}
