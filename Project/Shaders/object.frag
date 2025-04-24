#version 150
uniform sampler2D texUnit; 
uniform sampler2D ShadowMap;
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


void main(void)
{
	/* shadow mapping */ 
	vec4 shadowCoordinateWdivide = lightSourceCoord / lightSourceCoord.w ;
	
	// Used to lower moire pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;
	
	
	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
	
	
 	float shadow = 1.0;
 	if (lightSourceCoord.w > 0.0)
 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
  	


	/* light sources */ 

	vec3 loc_v1 = normalize(vec3((world_To_View * vec4(firePos, 1.0)) - SurfacePos)); 
	vec3 diff_Color = (max(0.0, dot(normalize(transformedNormal), loc_v1)) * fireColor);


	vec3 loc_v2 = normalize(vec3((world_To_View * vec4(moonPos, 1.0)) - SurfacePos)); 
	vec3 diff_Color2 = (max(0.0, dot(normalize(transformedNormal), loc_v2)) * moonColor);
	outColor = shadow * vec4(diff_Color + diff_Color2, 1.0) * texture(ShadowMap, outTexCord);
}
