#version 150
uniform sampler2D textUnit; 

in vec3 in_Position;
uniform mat4 mdlMatrix;
uniform mat4 projectionMatrix;
uniform mat4 worldCamera;
in vec3 inNormal; 
in vec2 inTexCord;
out vec2 outTexCord;

out vec4 SurfacePos;

out vec3 transformedNormal;
out vec3 exColor;




void main(void)
{
    gl_Position = projectionMatrix*worldCamera*mdlMatrix*vec4(in_Position, 1.0);
    const vec3 light = vec3(0.58, 0.58, 0.58);
    exColor = vec3(0.8,0,0.8);
    outTexCord = inTexCord;

    SurfacePos = worldCamera*mdlMatrix*vec4(in_Position, 1.0);

    transformedNormal = mat3(worldCamera)*mat3(mdlMatrix) * inNormal;
}
