#version 150
uniform sampler2D texUnit; 

uniform vec3 firePos;
uniform vec3 fireColor;

uniform vec3 moonDir;
uniform vec3 moonColor;

uniform mat4 model_To_World;
uniform mat4 world_To_View;
out vec4 outColor;
in vec3 transformedNormal;
in vec2 outTexCord;
in vec4 SurfacePos;
uniform sampler2D textureUnit;

in vec4 lightSourceCoord;

uniform sampler2D moonShadowMap; // Add this
in vec4 moonLightSourceCoord; // Add this

void main(void)
{
	// Perform perspective division to get the actual texture position
	vec4 shadowCoordinateWdivide = lightSourceCoord / lightSourceCoord.w;
	vec3 loc_v1 = normalize(vec3((world_To_View * vec4(firePos, 1.0)) - SurfacePos)); 
	vec3 fireDiffuse = (max(0.0, dot(normalize(transformedNormal), loc_v1)) * fireColor);


	// The optimal value here will vary with different GPU's depending on their Z buffer resolution.
	float bias = max(0.005 * (1.0 - dot(normalize(transformedNormal), loc_v1)), 0.001);  
	shadowCoordinateWdivide.z -= bias;

	// Look up the depth value
	float distanceFromLight = texture(textureUnit, shadowCoordinateWdivide.st).x;
	distanceFromLight = (distanceFromLight-0.5) * 2.0;


	float fireShadow = 1.0; // 1.0 = no shadow
	if (lightSourceCoord.w > 0.0)
		if (distanceFromLight < shadowCoordinateWdivide.z) // shadow
			fireShadow = 0.5;


	// Moonlight shadow
    vec4 moonShadowCoord = moonLightSourceCoord / moonLightSourceCoord.w;
    moonShadowCoord.z -= 0.005; // Bias
    float moonShadow = texture(moonShadowMap, moonShadowCoord.st).x;
    moonShadow = (moonShadow < moonShadowCoord.z) ? 0.5 : 1.0;

    // Moonlight diffuse
    vec3 moonDiffuse = max(0.0, dot(normalize(transformedNormal), -moonDir)) * moonColor;
    
    // Combine lights
    outColor = (fireShadow * vec4(fireDiffuse, 1.0) + moonShadow * vec4(moonDiffuse, 1.0)) * texture(texUnit, outTexCord);

}
