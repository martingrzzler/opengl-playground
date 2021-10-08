#include "ShaderProgram.h"
#include "utils.h"
#include <fmt/core.h>
#include <iostream>

ShaderProgram::ShaderProgram()
{
	_program_id = _uniform_model = _uniform_projection = _uniform_view = _uniform_point_light_count = 0;
	_point_light_count = 0;
}

GLuint ShaderProgram::model_location()
{
	return _uniform_model;
}

GLuint ShaderProgram::projection_location()
{
	return _uniform_projection;
}

GLuint ShaderProgram::specular_intensity_location()
{
	return _uniform_specular_intensity;
}
GLuint ShaderProgram::shininess_location()
{
	return _uniform_shininess;
}

GLuint ShaderProgram::eye_position_location()
{
	return _uniform_eye_position;
}

GLuint ShaderProgram::diffuse_intensity_location()
{
	return _directional_light.uniform_diffuse_intensity;
}

GLuint ShaderProgram::direction_location()
{
	return _directional_light.uniform_direction;
}

GLuint ShaderProgram::view_location()
{
	return _uniform_view;
}

GLuint ShaderProgram::color_location()
{
	return _directional_light.uniform_color;
}

GLuint ShaderProgram::ambient_intensity_location()
{
	return _directional_light.uniform_ambient_intensity;
}

void ShaderProgram::create_from_string(const char *v_shader_src, const char *f_shader_src)
{
	build(v_shader_src, f_shader_src);
}

void ShaderProgram::create_from_file(const char *v_shader_path, const char *f_shader_path)
{
	try
	{
		auto v_shader_src = utils::read_file_to_string(v_shader_path);
		auto f_shader_src = utils::read_file_to_string(f_shader_path);
		build(v_shader_src.c_str(), f_shader_src.c_str());
	}
	catch (const char *e)
	{
		std::cerr << e << '\n';
		throw e;
	}
}

void ShaderProgram::build(const char *v_shader_src, const char *f_shader_src)
{
	_program_id = glCreateProgram();
	if (!_program_id)
	{
		std::cerr << "ERROR: failed to create shader program" << std::endl;
		return;
	}

	compile_shader(v_shader_src, GL_VERTEX_SHADER);
	compile_shader(f_shader_src, GL_FRAGMENT_SHADER);

	GLint res = 0;
	GLchar msg[1024] = {0};
	glLinkProgram(_program_id);
	glGetProgramiv(_program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		glGetProgramInfoLog(_program_id, sizeof(msg), NULL, msg);
		std::cerr << "ERROR: Linking shaders - " << msg << std::endl;
		return;
	}
	glValidateProgram(_program_id);
	glGetProgramiv(_program_id, GL_VALIDATE_STATUS, &res);
	if (res == GL_FALSE)
	{
		glGetProgramInfoLog(_program_id, sizeof(msg), NULL, msg);
		std::cerr << "ERROR: Validating shaders - " << msg << std::endl;
		return;
	}
	_uniform_model = glGetUniformLocation(_program_id, "model");
	_uniform_projection = glGetUniformLocation(_program_id, "projection");
	_uniform_view = glGetUniformLocation(_program_id, "view");
	_directional_light.uniform_color = glGetUniformLocation(_program_id, "directional_light.color");
	_directional_light.uniform_ambient_intensity = glGetUniformLocation(_program_id, "directional_light.ambient_intensity");
	_directional_light.uniform_diffuse_intensity = glGetUniformLocation(_program_id, "directional_light.diffuse_intensity");
	_directional_light.uniform_direction = glGetUniformLocation(_program_id, "directional_light.direction");
	_uniform_eye_position = glGetUniformLocation(_program_id, "eye_position");
	_uniform_specular_intensity = glGetUniformLocation(_program_id, "material.specular_intensity");
	_uniform_shininess = glGetUniformLocation(_program_id, "material.shininess");

	_uniform_point_light_count = glGetUniformLocation(_program_id, "point_light_count");
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		std::string buf = fmt::format("point_lights[{}].base.color", i);
		_point_lights[i].uniform_color = glGetUniformLocation(_program_id, buf.c_str());
		buf = fmt::format("point_lights[{}].base.ambient_intensity", i);
		_point_lights[i].uniform_ambient_intensity = glGetUniformLocation(_program_id, buf.c_str());
		buf = fmt::format("point_lights[{}].base.diffuse_intensity", i);
		_point_lights[i].uniform_diffuse_intensity = glGetUniformLocation(_program_id, buf.c_str());
		buf = fmt::format("point_lights[{}].position", i);
		_point_lights[i].uniform_position = glGetUniformLocation(_program_id, buf.c_str());
		buf = fmt::format("point_lights[{}].constant", i);
		_point_lights[i].uniform_constant = glGetUniformLocation(_program_id, buf.c_str());
		buf = fmt::format("point_lights[{}].linear", i);
		_point_lights[i].uniform_linear = glGetUniformLocation(_program_id, buf.c_str());
		buf = fmt::format("point_lights[{}].exponent", i);
		_point_lights[i].uniform_exponent = glGetUniformLocation(_program_id, buf.c_str());
	}
}

void ShaderProgram::use_directional_light(const DirectionalLight &light)
{
	light.use(
			_directional_light.uniform_ambient_intensity,
			_directional_light.uniform_color,
			_directional_light.uniform_diffuse_intensity,
			_directional_light.uniform_direction);
}

void ShaderProgram::compile_shader(const char *shader_src, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shader_src, NULL);
	glCompileShader(shader);

	GLint res;
	char msg[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		glGetShaderInfoLog(shader, sizeof(msg), NULL, msg);
		std::cerr << "ERROR: Compiling shader " << type << "failed with message\n"
							<< msg << std::endl;
		return;
	}
	glAttachShader(_program_id, shader);
}

void ShaderProgram::use()
{
	glUseProgram(_program_id);
}
void ShaderProgram::unuse()
{
	glUseProgram(0);
}

void ShaderProgram::clear()
{
	if (_program_id != 0)
	{
		glDeleteProgram(_program_id);
		_program_id = 0;
	}
	_uniform_model = 0;
	_uniform_projection = 0;
}

ShaderProgram::~ShaderProgram()
{
	clear();
}