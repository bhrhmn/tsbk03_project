#version 330 core

layout (location = 0) in vec3 in_Position;

uniform mat4 world_To_View;
uniform mat4 projection;

void main()
{
    gl_Position = projection * world_To_View * vec4(in_Position, 1.0);
    gl_PointSize = 15.0;
}
