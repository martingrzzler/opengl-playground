#pragma once

#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

static const int MAX_POINT_LIGHTS = 3;
static const int MAX_SPOT_LIGHTS = 3;

class ShaderProgram
{
public:
	ShaderProgram();
	void create_from_string(const char *v_shader_src, const char *f_shader_src);
	void create_from_file(const char *v_shader_path, const char *f_shader_path);
	GLuint projection_location();
	GLuint model_location();
	GLuint view_location();
	GLuint ambient_intensity_location();
	GLuint color_location();
	GLuint diffuse_intensity_location();
	GLuint direction_location();
	GLuint shininess_location();
	GLuint specular_intensity_location();
	GLuint eye_position_location();

	void use_directional_light(const DirectionalLight &light);
	void use_point_lights(PointLight *lights, unsigned int length);
	void use_spot_lights(SpotLight *lights, unsigned int length);

	void use();
	void unuse();
	void clear();
	~ShaderProgram();

private:
	GLuint _program_id, _uniform_projection, _uniform_model,
			_uniform_view,
			_uniform_eye_position,
			_uniform_specular_intensity, _uniform_shininess;

	struct
	{
		GLuint uniform_color;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;
		GLuint uniform_direction;
	} _directional_light;

	struct
	{
		GLuint uniform_color;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;
		GLuint uniform_position;
		GLuint uniform_constant;
		GLuint uniform_linear;
		GLuint uniform_exponent;
	} _point_lights[MAX_POINT_LIGHTS];

	unsigned int _point_light_count;
	GLuint _uniform_point_light_count;

	struct
	{
		GLuint uniform_color;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;
		GLuint uniform_position;
		GLuint uniform_constant;
		GLuint uniform_linear;
		GLuint uniform_exponent;
		GLuint uniform_direction;
		GLuint uniform_edge;
	} _spot_lights[MAX_SPOT_LIGHTS];

	unsigned int _spot_light_count;
	GLuint _uniform_spot_light_count;

	void compile_shader(const char *shader_src, GLenum type);
	void build(const char *v_shader_src, const char *f_shader_src);
};