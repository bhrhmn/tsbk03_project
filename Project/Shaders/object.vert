#version 150
uniform sampler2D textUnit; 

in vec3 in_Position;
uniform mat4 projectionMatrix;
uniform mat4 model_To_World;
uniform mat4 world_To_View;
in vec3 inNormal; 
in vec2 inTexCord;
out vec2 outTexCord;

out vec4 SurfacePos;

out vec3 transformedNormal;
out vec3 exColor;




void main(void)
{
    gl_Position = projectionMatrix*world_To_View*model_To_World*vec4(in_Position, 1.0);
    const vec3 light = vec3(0.58, 0.58, 0.58);
    exColor = vec3(0.8,0,0.8);
    outTexCord = inTexCord;

    SurfacePos = world_To_View*model_To_World*vec4(in_Position, 1.0);

    transformedNormal = mat3(world_To_View)*mat3(model_To_World) * inNormal;
}
