// Shaders/shadow_cube.frag
#version 150
in vec4 FragPosGeo;

uniform vec3 lightPos;
uniform float far_plane;

// Use this for the depth buffer output
out float gl_FragDepth; // Or just use the built-in variable if your compiler accepts it

void main()
{
    // Calculate LINEAR distance from light (World units)
    float lightDistance = length(FragPosGeo.xyz - lightPos);

    // Normalize the linear distance to the [0, 1] range of the Z-buffer
    float depthValue = lightDistance / far_plane;

    // Write the normalized linear distance to the depth buffer
    gl_FragDepth = depthValue; // Note: gl_FragDepth is the built-in variable.

    // We are writing to the DEPTH ATTACHMENT, so we must also suppress color output
    // The FBO is set to glDrawBuffer(GL_NONE) so this is implicitly handled.
}