#version 150

in vec3 in_Position;
in vec3 inNormal; 
in vec2 inTexCord;

uniform mat4 projectionMatrix;
in mat4 scaleBiasMatrix;
uniform mat4 world_To_View;
uniform mat4 model_To_World;



void main(void)
{
    gl_Position = projectionMatrix * world_To_View * model_To_World* vec4(in_Position, 1.0);
}
