#version 150

in  vec3 in_Position;
uniform mat4 model_to_world;
uniform mat4 world_to_view;
uniform vec3 rain_pos[];
uniform mat4 projectionMatrix;

out vec2 texCoord;

void main(void)
{
	//gl_Position = projectionMatrix * world_to_view * vec4(rain_pos[gl_InstanceID], 1.0);
	gl_Position = projectionMatrix * world_to_view * model_to_world * vec4(in_Position, 1.0);
	
	//gl_Position = vec4(2*gl_InstanceID, 0.f, 0.f, 1.f);

	texCoord.s = in_Position.x+0.5;
	texCoord.t = in_Position.y+0.5;
}
