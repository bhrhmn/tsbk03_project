#version 150
uniform sampler2D textUnit; 

in vec3 in_Position;
uniform mat4 projectionMatrix;
uniform mat4 model_To_World;
uniform mat4 world_To_View;
uniform mat4 lightViewProjMatrix;
uniform mat4 lightViewProjMatrixMoon;  


in vec3 inNormal; 
in vec2 inTexCord;
out vec2 outTexCord;

uniform mat4 scaleBiasMatrix;
out vec4 lightSourceCoord;
out vec4 lightSourceCoordMoon;


out vec4 SurfacePos;

out vec3 transformedNormal;


void main(void)
{
    gl_Position = projectionMatrix*world_To_View*model_To_World*vec4(in_Position, 1.0);
   
    lightSourceCoord = scaleBiasMatrix * lightViewProjMatrix * model_To_World * vec4(in_Position, 1.0);
    lightSourceCoordMoon = scaleBiasMatrix * lightViewProjMatrixMoon * model_To_World * vec4(in_Position, 1.0);

    outTexCord = inTexCord;

    SurfacePos = world_To_View*model_To_World*vec4(in_Position, 1.0);

    transformedNormal = mat3(world_To_View)*mat3(model_To_World) * inNormal;
}