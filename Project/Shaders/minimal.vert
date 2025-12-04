#version 150

in  vec3 inPosition;
in vec2 inTexCord;
out vec2 texCoord;

void main(void)
{
	texCoord = inTexCord;
	
	gl_Position = vec4(inPosition, 1.0);
}
