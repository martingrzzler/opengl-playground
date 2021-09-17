#include "Mesh.h"

Mesh::Mesh()
{
	VAO = VBO = IBO = index_count = 0;
}

void Mesh::create(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices)
{
	index_count = num_of_indices;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * num_of_indices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sizeof(vertices[0])) * num_of_vertices, vertices, GL_STATIC_DRAW);

	// enable vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
	glEnableVertexAttribArray(0);
	// enable textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void *)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	unbind();
}

void Mesh::render()
{
	bind();
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	unbind();
}

void Mesh::bind()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void Mesh::clear()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	index_count = 0;
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