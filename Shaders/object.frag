#version 150
uniform sampler2D texUnit;
uniform sampler2D textureUnitMoon;
uniform samplerCube shadowCubeMap;

uniform vec3 firePos;
uniform vec3 fireColor;

uniform vec3 moonPos;
uniform vec3 moonColor;

uniform float far_plane;

uniform mat4 model_To_World;
uniform mat4 world_To_View;

out vec4 outColor;
in vec3 transformedNormal;
in vec2 outTexCord;
in vec4 SurfacePos;
in vec4 lightSourceCoord;

in vec4 lightSourceCoordMoon;
in vec3 fragPosWorld;

void setFireShadow(inout float shadow, inout vec3 diff_color_fire){

	vec3 fireLocation = normalize(vec3((world_To_View *vec4(firePos, 1.0)) - SurfacePos));
	diff_color_fire = (max(0.0, dot(normalize(transformedNormal), fireLocation)) * fireColor);

	vec3 fragToLight = fragPosWorld - firePos;

	// Sample from depth cube map
	float closestDepth = texture(shadowCubeMap, fragToLight).r;

	// Re-transform back to original value [0,far_plane]
	closestDepth *= far_plane;

	// Get current linear depth
	float currentDepth = length(fragToLight);

	// Test for shadows
	float bias = 0.05;
	if(currentDepth - bias > closestDepth){
		shadow -= 0.3f;
	}
}

void setFireShadow2(inout float shadow, inout vec3 diff_color_fire){
	vec3 fragToLight = fragPosWorld - firePos;

	// Sample from depth cube map
	float closestDepth = texture(shadowCubeMap, fragToLight).r;

	// DEBUG: Show the raw depth value from cube map
	// If this shows mostly black/dark, depth values are very small
	// If this shows mostly white, depth values are close to 1.0
	diff_color_fire = vec3(closestDepth);

	return; // Skip lighting and shadow calculation for now
}

void setMoonShadow(inout float shadow, inout vec3 diff_color_moon){
	vec3 moonLocation = normalize(vec3((world_To_View *vec4(moonPos, 1.0)) - SurfacePos));
	diff_color_moon = (max(0.0, dot(normalize(transformedNormal), moonLocation)) * moonColor);

	float biasMoon = max(0.005 * (1.0 - dot(normalize(transformedNormal), moonLocation)), 0.001);
	vec4 shadowCoordinateWdivideMoon = lightSourceCoordMoon / lightSourceCoordMoon.w;

	shadowCoordinateWdivideMoon.z -= biasMoon;

	float distanceFromLightMoon = texture(textureUnitMoon, shadowCoordinateWdivideMoon.st).x;
	distanceFromLightMoon = (distanceFromLightMoon-0.5) * 2.0;


	if (lightSourceCoordMoon.w > 0.0)
	if (distanceFromLightMoon < shadowCoordinateWdivideMoon.z){
		diff_color_moon = vec3(0, 0, 0);
		shadow -= 0.3;
	}
}

void main(void)
{
	float shadow = 1.0;
	vec3 diff_color_fire, diff_color_moon;

	setFireShadow(shadow, diff_color_fire);
	//setMoonShadow(shadow, diff_color_moon);

	//outColor =  shadow * vec4(diff_color_fire*0.8 + diff_color_moon,1.0) * texture(texUnit, outTexCord);
	outColor =  shadow * vec4(diff_color_fire*0.8,1.0) * texture(texUnit, outTexCord);


}
