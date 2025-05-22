#version 150
uniform sampler2D textUnit; 

in vec3 in_Position;
uniform mat4 model_To_World;
uniform mat4 projectionMatrix;
uniform mat4 world_To_View;
in vec3 inNormal; 
in vec2 inTexCord;
out vec2 outTexCord;

out vec3 transformedNormal;


void main(void)
{
    gl_Position = projectionMatrix*world_To_View*model_To_World*vec4(in_Position, 1.0);
    mat3 normalMatrix1 = mat3(model_To_World);
    const vec3 light = vec3(0.58, 0.58, 0.58);
    outTexCord = inTexCord;

    transformedNormal = normalMatrix1 * inNormal;
}
