#version 150
uniform sampler2D texUnit; 
uniform sampler2D textureUnit;
uniform sampler2D textureUnitMoon;

uniform vec3 firePos;
uniform vec3 fireColor;

uniform vec3 moonPos;
uniform vec3 moonColor;

uniform mat4 model_To_World;
uniform mat4 world_To_View;

out vec4 outColor;
in vec3 transformedNormal;
in vec2 outTexCord;
in vec4 SurfacePos;
in vec4 lightSourceCoord;

in vec4 lightSourceCoordMoon;


void main(void)
{

	vec3 fireLocation = normalize(vec3((world_To_View *vec4(firePos, 1.0)) - SurfacePos)); 
	vec3 moonLocation = normalize(vec3((world_To_View *vec4(moonPos, 1.0)) - SurfacePos)); 


	vec3 diff_color_fire = (max(0.0, dot(normalize(transformedNormal), fireLocation)) * fireColor);

	vec3 diff_color_moon = (max(0.0, dot(normalize(transformedNormal), moonLocation)) * moonColor);

	//fire
	vec4 shadowCoordinateWdivide = lightSourceCoord / lightSourceCoord.w;

	float bias = max(0.005 * (1.0 - dot(normalize(transformedNormal), fireLocation)), 0.001);  
	shadowCoordinateWdivide.z -= bias;

	float distanceFromLight = texture(textureUnit, shadowCoordinateWdivide.st).x;
	distanceFromLight = (distanceFromLight-0.5) * 2.0;

	float shadow = 1.0; // 1.0 = no shadow

	if (lightSourceCoord.w > 0.0)
		if (distanceFromLight < shadowCoordinateWdivide.z) // shadow
			shadow -= 0.3;


	//moon
	vec4 shadowCoordinateWdivideMoon = lightSourceCoordMoon / lightSourceCoordMoon.w;

	float biasMoon = max(0.005 * (1.0 - dot(normalize(transformedNormal), moonLocation)), 0.001);  
	shadowCoordinateWdivideMoon.z -= biasMoon;

	float distanceFromLightMoon = texture(textureUnitMoon, shadowCoordinateWdivideMoon.st).x;
	distanceFromLightMoon = (distanceFromLightMoon-0.5) * 2.0;


	if (lightSourceCoordMoon.w > 0.0)
		if (distanceFromLightMoon < shadowCoordinateWdivideMoon.z) // shadow
			diff_color_moon = vec3(0, 0, 0);
			shadow -= 0.3;
	

	outColor =  shadow * vec4(diff_color_fire*0.8 + diff_color_moon,1.0) *texture(texUnit, outTexCord);
}
