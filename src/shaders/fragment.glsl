#version 330

out vec4 color;
in vec4 vColor;
in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

const int MAX_POINT_LIGHTS = 3;


struct Light
{
	float ambient_intensity;
	vec3 color;
	float diffuse_intensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specular_intensity;
	float shininess;
};

uniform DirectionalLight directional_light;
uniform int point_light_count;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

uniform sampler2D texture_zero;
uniform Material material;

uniform vec3 eye_position;

vec4 calc_light_by_direction(Light light, vec3 direction)
{
	vec4 ambient_color = vec4(light.color, 1.0) * light.ambient_intensity;
	float diffuse_factor = max(dot(normalize(normal), normalize(direction)), 0.0);
	vec4 diffuse_color = vec4(light.color, 1.0) * light.diffuse_intensity * diffuse_factor;

	vec4 specular_color = vec4(0,0,0,0);
	if (diffuse_factor > 0.0)
	{
		vec3 frag_to_eye = normalize(eye_position - frag_pos);
		vec3 reflected_vec = normalize(reflect(-direction, normalize(normal)));
		float specular_factor = dot(frag_to_eye, reflected_vec);
		if (specular_factor > 0.0)
		{
			specular_factor = pow(specular_factor, material.shininess);
			specular_color = vec4(light.color * material.specular_intensity * specular_factor, 1.0);
		}
	}

	return (ambient_color + diffuse_color + specular_color);
}

vec4 calc_directional_light()
{
	return calc_light_by_direction(directional_light.base, directional_light.direction);
}

vec4 calc_point_lights()
{
	vec4 total_color = vec4(0,0,0,0);
	for (int i = 0; i < point_light_count; i++)
	{
		vec3 direction =  point_lights[i].position - frag_pos;
		float dist = length(direction);
		direction = normalize(direction);

		vec4 color = calc_light_by_direction(point_lights[i].base, direction);
		// ax^2+bx+c
		float attenuation = point_lights[i].exponent * dist * dist + point_lights[i].linear * dist + point_lights[i].constant; 
		total_color += (color / attenuation);
	}

	return total_color;
}

void main()
{

	vec4 light_color = calc_directional_light();

	color = texture(texture_zero, tex_coord) * light_color;
}