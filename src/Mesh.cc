#include "Mesh.h"
#include <exception>
#include <fmt/core.h>
#include <iostream>

Mesh::Mesh(
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures) : _vertices(vertices), _indices(indices), _textures(textures)
{
	create();
}

void Mesh::draw(ShaderProgram *shader)
{
	unsigned int diffuse_nr = 1;
	unsigned int specular_nr = 1;

	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		unsigned int num = 0;
		glActiveTexture(GL_TEXTURE0 + i);
		auto type = _textures[i].type();
		switch (type)
		{
		case TextureMaterial::DIFFUSE:
			num = diffuse_nr++;
			break;
		case TextureMaterial::SPECULAR:
			num = specular_nr++;
			break;
		default:
			std::cout << fmt::format("Unsupported TextureMaterial: {}", Texture::texture_type_to_string(type)) << std::endl;
			break;
		}
		// e.g. texture_diffuse1
		shader->use_int(fmt::format("{}{}", Texture::texture_type_to_string(type), num), i);
		_textures[i].use();
	}

	bind();
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	unbind();
}

void Mesh::create()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

	// enable vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	//enable normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	// enable textures coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_cords));
	glEnableVertexAttribArray(1);

	unbind();
}

void Mesh::bind()
{
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void Mesh::clear()
{
	// if (_ebo != 0)
	// {
	// 	glDeleteBuffers(1, &_ebo);
	// 	_ebo = 0;
	// }
	// if (_vbo != 0)
	// {
	// 	glDeleteBuffers(1, &_vbo);
	// 	_vbo = 0;
	// }
	// if (_vao != 0)
	// {
	// 	glDeleteVertexArrays(1, &_vao);
	// 	_vao = 0;
	// }
}

void Mesh::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	clear();
}