#version 460 core

#define MAX_LIGHTS	5
#define POINT		0
#define DIRECTIONAL 1
#define SPOT		2

in VS_OUT
{
	vec2 texcoord;
	vec3 position;
	vec3 normal;
} fs_in;

out vec4 f_color;

struct Material
{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
};

struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	float innerSpotAngle;
	float outerSpotAngle;
};

uniform Material u_material;
uniform int u_numLights = 1;
uniform Light u_lights[MAX_LIGHTS];

uniform vec3 u_ambient_light;

float calculateAttenuation(in float light_distance, in float range)
{
	float attenuation = max(0.0, (1.0 - (light_distance / range)));
	return pow(attenuation, 2.0);
}

vec3 calculateLight(in Light light, in vec3 position, in vec3 normal)
{
	float attenuation = 1.0;
	vec3 light_dir;

	switch (light.type)
	{
		case POINT:
			{
			light_dir = normalize(light.position - position);
			
			float light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);
			}
		break;	
		case DIRECTIONAL:
			light_dir = light.direction;
		break;	
		case SPOT:
			{
			light_dir = normalize(light.position - position);

			float light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);

			float angle = acos(dot(light_dir, light.direction));
			if (angle > light.outerSpotAngle) attenuation = 0.0;
			else {
				float spotAttenuation = smoothstep(light.outerSpotAngle + 0.001, light.innerSpotAngle, angle);
				attenuation *= spotAttenuation;
			}
			}
		break;	
	}

	// diffuse
	float NdotL = max(dot(normal, light_dir), 0);
	vec3 diffuse = light.color * u_material.baseColor * NdotL;

	// specular
	vec3 view_dir = normalize(-position);

	// blinn-phong 
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float NdotH = max(dot(normal, halfway_dir), 0);
	NdotH = pow(NdotH, u_material.shininess);
	vec3 specular = vec3(NdotH);

	// phong
	//vec3 reflection = reflect(-light_dir, normal);
	//float RdotV = max(dot(reflection, view_dir), 0);
	//RdotV = pow(RdotV, u_material.shininess);
	//vec3 specular = vec3(RdotV);

	return (diffuse + specular) * light.intensity * attenuation;
}

void main()
{
	//vec3 color = calculateLight(fs_in.position, fs_in.normal);
	//f_color = texture(u_material.baseMap, fs_in.texcoord) * vec4(color, 1);

	vec3 color = u_ambient_light;
	for (int i = 0; i < u_numLights; i++)
	{
		color += calculateLight(u_lights[i], fs_in.position, fs_in.normal);
	}

	f_color = texture(u_material.baseMap, fs_in.texcoord) * vec4(color, 1);
}
