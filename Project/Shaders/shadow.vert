// Used for shadow lookup
varying vec4 ShadowCoord;


uniform mat4 projectionMatrix;
uniform mat4 model_To_World;
uniform mat4 world_To_View;

in vec3 inNormal; 
in vec2 inTexCord;
in vec3 in_Position;


void main()
{


     	ShadowCoord= projectionMatrix * gl_Vertex;
  
		gl_Position = projectionMatrix * world_To_View * model_To_World * gl_Vertex;

		gl_FrontColor = gl_Color;
}

