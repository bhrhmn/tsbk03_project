#version 150
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;

uniform bool isDirectional[4];
uniform sampler2D texUnit;
uniform int isSky;

uniform mat4 worldToView;

const vec4 light = vec4(0.58, 0.58, 0.58, 1.0);

in vec4 surface_Position;
in vec2 frag_TexCoord;
in vec3 frag_Normal;

out vec4 out_Color;

void main(void)
{	
	if (isSky == 1) {

		out_Color =  texture(texUnit, frag_TexCoord);
	
	} else if (isSky == 2) { 
		
		/* Ambient light */ 
		
		vec3 amb_Color = light.xyz;
		
		vec3 diff_Color = vec3(0.0, 0.0, 0.0);
		vec3 spec_Color = vec3(0.0, 0.0, 0.0);

		for (int i = 0; i < 4; i++)
		{
			if (isDirectional[i]) 
			{
				diff_Color += (max(0.0, dot(normalize(frag_Normal), mat3(worldToView) * lightSourcesDirPosArr[i])) * lightSourcesColorArr[i]);
				
				vec3 spec_vec_dir = reflect(mat3(worldToView) * -lightSourcesDirPosArr[i], normalize(frag_Normal));
				float cos_phi_dir = dot(spec_vec_dir, -vec3(normalize(surface_Position)));
				spec_Color += lightSourcesColorArr[i] * pow(max(0.0, cos_phi_dir), 100.0);
			} 
			else 
			{
				vec3 loc_v1 = normalize(vec3((worldToView * vec4(lightSourcesDirPosArr[i], 1.0)) - surface_Position)); 
				diff_Color += (max(0.0, dot(normalize(frag_Normal), loc_v1)) * lightSourcesColorArr[i]);

				vec3 spec_vec_pos = reflect(-normalize(vec3(worldToView * vec4(lightSourcesDirPosArr[i], 1.0)) - vec3(surface_Position)), normalize(frag_Normal));
				float cos_phi_pos = dot(spec_vec_pos, -vec3(normalize(surface_Position)));
				spec_Color += lightSourcesColorArr[i] * pow(max(0.0, cos_phi_pos), 100.0);
			}
		}	
		
		out_Color = vec4(0.5 * amb_Color + 0.5 * diff_Color + 9.9 * spec_Color, 1.0) * texture(texUnit, frag_TexCoord);;

	} else {
	
		/* Ambient light */ 
		
		vec3 amb_Color = light.xyz;
		
		vec3 diff_Color = vec3(0.0, 0.0, 0.0);
		vec3 spec_Color = vec3(0.0, 0.0, 0.0);

		for (int i = 0; i < 4; i++)
		{
			if (isDirectional[i]) 
			{
				diff_Color += (max(0.0, dot(normalize(frag_Normal), mat3(worldToView) * lightSourcesDirPosArr[i])) * lightSourcesColorArr[i]);
				
				vec3 spec_vec_dir = reflect(mat3(worldToView) * -lightSourcesDirPosArr[i], normalize(frag_Normal));
				float cos_phi_dir = dot(spec_vec_dir, -vec3(normalize(surface_Position)));
				spec_Color += lightSourcesColorArr[i] * pow(max(0.0, cos_phi_dir), specularExponent);
			} 
			else 
			{
				vec3 loc_v1 = normalize(vec3((worldToView * vec4(lightSourcesDirPosArr[i], 1.0)) - surface_Position)); 
				diff_Color += (max(0.0, dot(normalize(frag_Normal), loc_v1)) * lightSourcesColorArr[i]);

				vec3 spec_vec_pos = reflect(-normalize(vec3(worldToView * vec4(lightSourcesDirPosArr[i], 1.0)) - vec3(surface_Position)), normalize(frag_Normal));
				float cos_phi_pos = dot(spec_vec_pos, -vec3(normalize(surface_Position)));
				spec_Color += lightSourcesColorArr[i] * pow(max(0.0, cos_phi_pos), specularExponent);
			}
		}	
		
		out_Color = vec4(0.0 * amb_Color + 0.5 * diff_Color + 0.8 * spec_Color, 1.0);
	}
}
