#version 150

in vec3 in_Position;
in vec3 inNormal;
uniform mat4 lightViewProjMatrix;

uniform mat4 model_To_World;


void main(void)
{
    gl_Position = lightViewProjMatrix * model_To_World * vec4(in_Position, 1.0);
}
