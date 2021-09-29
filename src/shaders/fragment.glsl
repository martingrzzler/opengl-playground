#version 330

out vec4 color;
in vec4 vColor;
in vec2 tex_coord;
in vec3 normal;

struct DirectionalLight
{
	float ambient_intensity;
	vec3 color;
	vec3 direction;
	float diffuse_intensity;
};

uniform sampler2D texture_zero;
uniform DirectionalLight directional_light;

void main()
{
	vec4 ambient_color = vec4(directional_light.color, 1.0) * directional_light.ambient_intensity;
	float diffuse_factor = max(dot(normalize(normal), normalize(directional_light.direction)), 0.0);
	vec4 diffuse_color = vec4(directional_light.color, 1.0) * directional_light.diffuse_intensity * diffuse_factor;
	color = texture(texture_zero, tex_coord) * (ambient_color + diffuse_color);
}