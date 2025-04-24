// Used for shadow lookup

uniform mat4 projectionMatrix;
uniform mat4 model_To_World;
uniform mat4 world_To_View;

in vec3 inNormal; 
in vec2 inTexCord;
in vec3 in_Position;


void main()
{

	gl_Position = projectionMatrix*world_To_View*model_To_World*vec4(in_Position, 1.0);
}

