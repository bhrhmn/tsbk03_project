#version 150
uniform sampler2D texUnit; 

uniform vec3 firePos;
uniform vec3 fireColor;
uniform mat4 worldCamera;
out vec4 outColor;
uniform mat4 myMatrix;
in vec3 exColor;
in vec3 transformedNormal;
in vec2 outTexCord;
in vec4 SurfacePos;


void main(void)
{
	/* do light here */ 
	


	vec3 loc_v1 = normalize(vec3((worldCamera * vec4(firePos, 1.0)) - SurfacePos)); 
	vec3 diff_Color = (max(0.0, dot(normalize(transformedNormal), loc_v1)) * fireColor);
	outColor = vec4(diff_Color, 1.0) * texture(texUnit, outTexCord);
}
