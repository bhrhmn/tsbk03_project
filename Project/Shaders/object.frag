#version 150
uniform sampler2D texUnit; 
uniform sampler2D textureUnit;

uniform vec3 firePos;
uniform vec3 fireColor;
uniform mat4 model_To_World;
uniform mat4 world_To_View;

out vec4 outColor;
in vec3 transformedNormal;
in vec2 outTexCord;
in vec4 SurfacePos;
in vec4 lightSourceCoord;


void main(void)
{

	vec3 loc_v1 = normalize(vec3((world_To_View * model_To_World *vec4(firePos, 1.0)) - SurfacePos)); 
	vec3 diff_Color = (max(0.0, dot(normalize(transformedNormal), loc_v1)) * fireColor);

	
	vec4 shadowCoordinateWdivide = lightSourceCoord / lightSourceCoord.w;

	float bias = max(0.005 * (1.0 - dot(normalize(transformedNormal), loc_v1)), 0.001);  
	shadowCoordinateWdivide.z -= bias;

	float distanceFromLight = texture(textureUnit, shadowCoordinateWdivide.st).x;
	//distanceFromLight = (distanceFromLight-0.5) * 2.0;

	float shadow = 1.0; // 1.0 = no shadow

	if (lightSourceCoord.w > 0.0)
		if (distanceFromLight < shadowCoordinateWdivide.z) // shadow
			shadow = 0.5;


	outColor = shadow * vec4(diff_Color, 1.0) * texture(texUnit, outTexCord);
}
