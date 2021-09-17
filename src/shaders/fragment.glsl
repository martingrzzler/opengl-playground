#version 330

out vec4 color;
in vec4 vColor;
in vec2 tex_coord;

uniform sampler2D texture_zero;

void main()
{
	color = texture(texture_zero, tex_coord) * vColor;
}