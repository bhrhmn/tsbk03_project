#version 150

uniform sampler2D texUnit;
const vec4 light = vec4(1.0, 1.0, 1.0, 1.0);

in vec2 frag_TexCoord;
in vec4 frag_Normal;

out vec4 out_Color;

void main(void)
{	
	out_Color = vec4(light.zzz, 1.0) * texture(texUnit, frag_TexCoord);
}
