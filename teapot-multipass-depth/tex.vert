#version 150

// Simple example with transformation matrices (too many, really,
// they should be multiplied on the CPU)

in  vec3 in_Position;
in  vec2 in_Texcoord;
in  vec3 in_Normal;
uniform mat4 modelView;
uniform mat4 projMatrix;
out  vec2 texcoord;
out  vec3 exNormal;

void main(void)
{
	gl_Position = projMatrix * modelView * vec4(in_Position, 1.0);
	exNormal = mat3(modelView) * in_Normal;
	texcoord = in_Texcoord;
}
