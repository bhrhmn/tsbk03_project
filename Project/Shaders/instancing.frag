#version 150

out vec4 out_Color;
uniform sampler2D tex;
uniform sampler2D pos1;
uniform sampler2D pos2;
in vec2 texCoord;

void main(void)
{
	// debugging things:
	// vec4 t = texture(tex, texCoord);
	// vec4 p1 = texture(pos1, texCoord);
	// vec4 p2 = texture(pos2, texCoord);

	// create circle
	float radius = 0.3;
	float delta_x = abs(texCoord.x - 0.5);
	float delta_y = abs(texCoord.y - 0.5);
	float dist_middle = sqrt(pow(delta_x, 2) + pow(delta_y, 2));
	if (dist_middle > radius)
		discard;

	out_Color = vec4(0.2, 0.2, 0.9, 0.5);
}
