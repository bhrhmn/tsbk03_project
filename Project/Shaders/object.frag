#version 150
uniform sampler2D texUnit; 

uniform vec3 firePos;
uniform vec3 fireColor;
uniform mat4 model_To_World;
uniform mat4 world_To_View;
out vec4 outColor;
in vec3 transformedNormal;
in vec2 outTexCord;
in vec4 SurfacePos;
uniform sampler2D textureUnit;

in vec4 lightSourceCoord;


void main(void)
{
	/* do light here */ 
	


	vec3 loc_v1 = normalize(vec3((world_To_View * vec4(firePos, 1.0)) - SurfacePos)); 
	vec3 diff_Color = (max(0.0, dot(normalize(transformedNormal), loc_v1)) * fireColor);
	outColor = vec4(diff_Color, 1.0) * texture(texUnit, outTexCord);
}
