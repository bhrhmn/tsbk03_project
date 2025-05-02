#version 150

in vec3 in_Position;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
in mat4 scaleBiasMatrix;
uniform mat4 world_To_View;



void main(void)
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(in_Position, 1.0);
}
