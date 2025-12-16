// Shaders/shadow_cube.frag
#version 150
in vec4 FragPosGeo;

uniform vec3 lightPos;
uniform float far_plane;

void main() {
    // 1. Calculate REAL Distance (Not just X)
    float lightDistance = length(FragPosGeo.xyz - lightPos);

    // 2. Normalize by the SAME number as your object shader (25.0)
    gl_FragDepth = lightDistance / far_plane;
}