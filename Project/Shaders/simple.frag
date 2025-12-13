#version 150

out vec4 out_Color;
in vec2 texCoord;
uniform sampler2D textureUnit;

void main(void)
{
    vec4 t = texture(textureUnit, texCoord);
	out_Color = vec4(t.x, 0.0, t.z, t.w);
}
