#version 150

uniform mat4 rotationMatrix;
uniform mat4 ModelToWorld;
uniform mat4 projection;
uniform mat4 worldToView;

in vec3 in_Position;
in vec2 inTexCoord;
in vec3 in_Normal;

out vec4 frag_Normal;
out vec4 frag_Colors;
out vec2 frag_TexCoord;

void main(void)
{	
	gl_Position = projection * worldToView * ModelToWorld * vec4(in_Position, 1.0);
	frag_Normal = ModelToWorld * vec4(in_Normal, 1.0);
	frag_TexCoord = inTexCoord;
}
