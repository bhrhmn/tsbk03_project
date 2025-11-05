#version 150

uniform float shade;
out vec4 out_Color;
in vec2 texCoord;
uniform sampler2D textureUnit;
uniform mat4 world_To_View;
uniform mat4 scaleBiasMatrix;

void main(void)
{
	out_Color = texture(textureUnit, texCoord);
}
