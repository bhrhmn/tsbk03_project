#version 150
in vec4 FragPosGeo;

uniform vec3 lightPos;
uniform float far_plane;

out vec4 out_Color;

void main()
{
    // Calculate distance from light source
    vec3 worldPos = FragPosGeo.xyz;
    float lightDistance = length(worldPos - lightPos);

    // DEBUG: Check if distances are reasonable
    // They should range from very small (near light) to far_plane

    // Normalize to [0,1] range
    lightDistance = lightDistance / far_plane;

    // Clamp to avoid issues
    lightDistance = clamp(lightDistance, 0.0, 1.0);

    out_Color = vec4(lightDistance, lightDistance, lightDistance, 1.0);
}