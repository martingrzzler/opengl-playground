#include "Texture.h"
#include <exception>
#include <fmt/core.h>
#include <iostream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(std::string directory, std::string file_name, TextureMaterial type)
		: _id(0), _directory(directory), _file_name(file_name), _type(type)
{
	_load();
}

void Texture::_load()
{
	// or set it with assimp
	stbi_set_flip_vertically_on_load(true);
	int width, height, bit_depth;
	auto file_path = fmt::format("{}/{}", _directory, _file_name);
	auto tex_data = stbi_load(file_path.c_str(), &width, &height, &bit_depth, STBI_rgb_alpha);
	if (!tex_data)
	{
		throw std::runtime_error(fmt::format("ERROR could not open file: {}/{}", _directory, _file_name));
	}

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(tex_data);
}

void Texture::clear()
{
	glDeleteTextures(1, &_id);
	_id = 0;
	_directory = "";
	_file_name = "";
}

std::string Texture::texture_type_to_string(TextureMaterial type)
{
	switch (type)
	{
	case TextureMaterial::DIFFUSE:
		return "texture_diffuse";
	case TextureMaterial::SPECULAR:
		return "texture_specular";
	default:
		throw std::runtime_error("Invalid TextureMaterial");
	}
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, _id);
}

TextureMaterial Texture::type()
{
	return _type;
}

std::string Texture::file_name()
{
	return _file_name;
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	clear();
}