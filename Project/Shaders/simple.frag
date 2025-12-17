#version 150

out vec4 out_Color;
in vec2 texCoord;
uniform sampler2D textureUnit;

void main(void)
{
    vec4 t = texture(textureUnit, texCoord);
	out_Color = vec4(t.x, t.y, t.z, 1.0);
}
