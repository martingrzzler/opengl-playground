#version 330

out vec4 color;
in vec4 vColor;
in vec2 tex_coord;

struct DirectionalLight
{
	float intensity;
	vec3 color;
};

uniform sampler2D texture_zero;
uniform DirectionalLight directional_light;

void main()
{
	vec4 ambient_color = vec4(directional_light.color, 1.0) * directional_light.intensity;
	color = texture(texture_zero, tex_coord) * vColor * ambient_color;
}