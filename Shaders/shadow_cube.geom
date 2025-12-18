#version 150
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowTransforms[6];
in vec4 FragPos[];
out vec4 FragPosGeo;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i)
        {
            // Read from the BLOCK array
            FragPosGeo = FragPos[i];

            gl_Position = shadowTransforms[face] * FragPosGeo;
            EmitVertex();
        }
        EndPrimitive();
    }
}