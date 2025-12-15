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

// In object.frag, replace setFireShadow with this debug version:
void setFireShadow(inout float shadow, inout vec3 diff_color_fire){
	// 1. Vector from Fragment to Light
	vec3 fragToLight = firePos - fragPosWorld; // Vector FROM light TO fragment
	float currentDepth = length(fragToLight);

	// 2. Retrieve Closest Depth (Normalized [0, 1] value)
	float closestDepthNormalized = texture(shadowCubeMap, fragToLight).r;

	// 3. Un-normalize the stored depth to get World Units
	// The stored value must be multiplied by far_plane to get world units.
	float closestDepthWorld = closestDepthNormalized * far_plane;

	float bias = 0.05; // Use a reasonable bias

	// Compare World Units (current fragment) vs. World Units (stored depth)
	if (currentDepth > closestDepthWorld + bias) {
		shadow = 0.0;
	}

	// Keep normal lighting calculation
	vec3 fireLocation = normalize(vec3((world_To_View * vec4(firePos, 1.0)) - SurfacePos));
	diff_color_fire = (max(0.0, dot(normalize(transformedNormal), fireLocation)) * fireColor);
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
