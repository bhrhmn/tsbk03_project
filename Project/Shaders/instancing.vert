#version 150

in vec3 inPosition;
in vec2 inTexCoord;
uniform mat4 model_to_world;
uniform mat4 world_to_view;
uniform mat4 projectionMatrix;
uniform float texSize;	
uniform vec3 pos_offset;
uniform sampler2D pos1;

out vec2 texCoord;

void main(void)
{
	int i = gl_InstanceID;
	float x = float(i) / texSize;
	float y = float(i % int(texSize));
	vec2 tc = vec2((float(x)+0.5) / texSize, (float(y)+0.5) / texSize);
	vec4 p = texture(pos1, tc) * 2000.0;
	vec3 p_moved = vec3(p.x, p.y, p.z) - pos_offset;
	vec4 pos = vec4(p_moved, 1.0);	

	pos = world_to_view * pos;
	mat4 view = world_to_view;
	
	view[3][0] += pos.x;
	view[3][1] += pos.y;
	view[3][2] += pos.z;

	gl_Position = projectionMatrix * view * vec4(inPosition, 1.0);
	
	texCoord = inTexCoord;
}
