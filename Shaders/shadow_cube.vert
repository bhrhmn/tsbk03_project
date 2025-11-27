#version 150
in vec3 in_Position;

uniform mat4 model_To_World;

out vec4 FragPos;

void main()
{
    FragPos = model_To_World * vec4(in_Position, 1.0);
}