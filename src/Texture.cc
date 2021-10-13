#include "Texture.h"
#include <exception>
#include <fmt/core.h>
#include <iostream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
		: _id(0), _width(0), _height(0), _bit_depth(0), _file_path("")
{
}

Texture::Texture(const char *file_path)
		: _id(0), _width(0), _height(0), _bit_depth(0), _file_path(file_path)
{
}

void Texture::load()
{
	auto tex_data = stbi_load(_file_path, &_width, &_height, &_bit_depth, STBI_rgb_alpha);
	if (!tex_data)
	{
		throw std::runtime_error(fmt::format("ERROR could not open file: {}", _file_path));
	}

	glGenTextures(1, &_id);
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	unbind();
	stbi_image_free(tex_data);
}

void Texture::clear()
{
	glDeleteTextures(1, &_id);
	_id = 0;
	_width = 0;
	_height = 0;
	_bit_depth = 0;
	_file_path = "";
}

void Texture::use()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);
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