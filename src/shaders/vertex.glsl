#version 330
	
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 vColor;
out vec2 tex_coord;
out vec3 normal;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vColor = vec4(clamp(pos, 0.0, 1.0), 1.0);
	tex_coord = tex;
	normal = mat3(transpose(inverse(model))) * norm;
}