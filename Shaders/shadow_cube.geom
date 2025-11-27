#version 150
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowTransforms[6];

in vec4 FragPos[]; // This should be world position
out vec4 FragPosGeo;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i)
        {
            FragPosGeo = FragPos[i]; // Pass world position to fragment shader
            gl_Position = shadowTransforms[face] * FragPos[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}