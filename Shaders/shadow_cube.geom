#version 150
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;
uniform mat4 shadowTransforms[6];
in vec4 FragPos[];
// Use noperspective for the world-space position to prevent incorrect interpolation.
noperspective out vec4 FragPosGeo; // <--- ADDED noperspective

void main() {
    for(int face = 0; face < 6; ++face) {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i) {
            FragPosGeo = FragPos[i];
            gl_Position = shadowTransforms[face] * FragPos[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}