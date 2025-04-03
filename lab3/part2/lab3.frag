#version 150

uniform sampler2D texUnit;
const vec4 light = vec4(2.58, 4.58, -2.58, 1.0);

in vec2 frag_TexCoord;
in vec4 frag_Normal;

out vec4 out_Color;

void main(void)
{	
	out_Color = dot(frag_Normal, normalize(light)) * texture(texUnit, frag_TexCoord);
}
