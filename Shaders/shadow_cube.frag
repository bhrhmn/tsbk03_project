// Shaders/shadow_cube.frag
#version 150
in vec4 FragPosGeo;

uniform vec3 lightPos;
uniform float far_plane;

void main() {
    float lightDistance = length(FragPosGeo.xyz - lightPos);

    gl_FragDepth = lightDistance / far_plane;
}