#version 330

out vec4 color;
in vec4 vColor;
in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

struct DirectionalLight
{
	float ambient_intensity;
	vec3 color;
	vec3 direction;
	float diffuse_intensity;
};

struct Material
{
	float specular_intensity;
	float shininess;
};

uniform sampler2D texture_zero;
uniform DirectionalLight directional_light;
uniform Material material;

uniform vec3 eye_position;

void main()
{
	vec4 ambient_color = vec4(directional_light.color, 1.0) * directional_light.ambient_intensity;
	float diffuse_factor = max(dot(normalize(normal), normalize(directional_light.direction)), 0.0);
	vec4 diffuse_color = vec4(directional_light.color, 1.0) * directional_light.diffuse_intensity * diffuse_factor;

	vec4 specular_color = vec4(0,0,0,0);
	if (diffuse_factor > 0.0)
	{
		vec3 frag_to_eye = normalize(eye_position - frag_pos);
		vec3 reflected_vec = normalize(reflect(directional_light.direction, normalize(normal)));
		float specular_factor = dot(frag_to_eye, reflected_vec);
		if (specular_factor > 0.0)
		{
			specular_factor = pow(specular_factor, material.shininess);
			specular_color = vec4(directional_light.color * material.specular_intensity * specular_factor, 1.0);
		}
	}


	color = texture(texture_zero, tex_coord) * (ambient_color + diffuse_color + specular_color);
}