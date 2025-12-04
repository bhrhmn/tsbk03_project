#version 150

in vec3 inPosition;
in vec2 inTexCord;
uniform mat4 model_to_world;
uniform mat4 world_to_view;
uniform mat4 projectionMatrix;
uniform sampler2D posTex;
uniform int texSize;	

out vec2 texCoord;

void main(void)
{
	int i = gl_InstanceID;
	float x = float(i) / texSize;
	float y = i % texSize;
	vec2 tc = vec2((float(x)+0.5) / texSize, (float(y)+0.5) / texSize);
	//vec2 tc = vec2(x, y);
	vec4 p = texture(posTex, tc) * 1000;
	vec4 pos = vec4(p.x, p.y, p.z, 1);	
	// pos[3] = 1;

	//pos = pos + vec4(gl_InstanceID*15, 0, 0, 0);
	pos = world_to_view * pos;
	mat4 view = world_to_view;
	
	view[3][0] += pos.x;
	view[3][1] += pos.y;
	view[3][2] += pos.z;

// Eliminate rotations
	view[0][0] = 1;
	view[1][0] = 0;
	view[2][0] = 0;
	view[0][1] = 0;
	view[1][1] = 1;
	view[2][1] = 0;
	view[0][2] = 0;
	view[1][2] = 0;
	view[2][2] = 1;
	gl_Position = projectionMatrix * view * vec4(inPosition, 1.0);
	

	texCoord = inTexCord;
}
