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
	
	// Perform perspective division to get the actual texture position
	vec4 shadowCoordinateWdivide = lightSourceCoord / lightSourceCoord.w;
	vec3 loc_v1 = normalize(vec3((world_To_View * vec4(firePos, 1.0)) - SurfacePos)); 
	vec3 diff_Color = (max(0.0, dot(normalize(transformedNormal), loc_v1)) * fireColor);

	// Used to lower moire' pattern and self-shadowing
	// The optimal value here will vary with different GPU's depending on their Z buffer resolution.
	float bias = max(0.005 * (1.0 - dot(normalize(transformedNormal), loc_v1)), 0.001);  
	shadowCoordinateWdivide.z -= bias;

	// Look up the depth value
	float distanceFromLight = texture(textureUnit, shadowCoordinateWdivide.st).x;
	distanceFromLight = (distanceFromLight-0.5) * 2.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(textureUnit, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float depth = texture(textureUnit, shadowCoordinateWdivide.st + vec2(x, y) * texelSize).x; 
			depth = (depth - 0.5) * 2.0;  // Assuming depth was stored in [-1,1]
			shadow += (depth < shadowCoordinateWdivide.z) ? 0.5 : 1.0;
		}
	}
	shadow /= 9.0;


	outColor = shadow * vec4(diff_Color, 1.0) * texture(texUnit, outTexCord);
}
