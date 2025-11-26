#version 150

in vec3 in_Position;
in vec2 inTexCord;
uniform mat4 model_to_world;
uniform mat4 world_to_view;
//uniform vec3 rain_pos[];
uniform mat4 projectionMatrix;
uniform sampler2D pos1;
uniform int texSize;	

out vec2 texCoord;

void main(void)
{
	int i = gl_InstanceID;
	int x = i / texSize;
	int y = i % texSize;

	vec4 pos = texture(pos1, vec2(x, y));	

	pos = vec4(gl_InstanceID*15, 0, 0, 1);
	pos = world_to_view * pos;
	mat4 view = world_to_view;
	
	view[3][0] += pos.x;
	view[3][1] += pos.y;
	view[3][2] += pos.z;

	//gl_Position = projectionMatrix * world_to_view * vec4(rain_pos[gl_InstanceID], 1.0);
	gl_Position = projectionMatrix * view * vec4(in_Position, 1.0);
	
	//gl_Position = vec4(2*gl_InstanceID, 0.f, 0.f, 1.f);

	// texCoord.s = in_Position.x+0.5;
	// texCoord.t = in_Position.y+0.5;

	texCoord = inTexCord;
}
