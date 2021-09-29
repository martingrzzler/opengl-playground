#include "utils.h"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <sstream>

namespace utils
{
	std::string read_file_to_string(const char *path)
	{
		std::ifstream stream(path);
		if (!stream.is_open())
			throw "ERROR: file not found";

		std::stringstream buf;
		buf << stream.rdbuf();
		return buf.str();
	}

	void calc_avg_normals(
			unsigned int *indices,
			unsigned int indices_count,
			GLfloat *vertices,
			unsigned int vertices_count,
			unsigned int vertex_length,
			unsigned int normal_offset)
	{
		// add up all normals
		for (size_t i = 0; i < indices_count; i += 3)
		{
			unsigned int in0 = indices[i] * vertex_length;
			unsigned int in1 = indices[i + 1] * vertex_length;
			unsigned int in2 = indices[i + 2] * vertex_length;
			glm::vec3 v1_minus_v0(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
			glm::vec3 v2_minus_v0(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
			glm::vec3 normal = glm::cross(v1_minus_v0, v2_minus_v0);
			normal = glm::normalize(normal);

			in0 += normal_offset;
			in1 += normal_offset;
			in2 += normal_offset;

			vertices[in0] += normal.x;
			vertices[in0 + 1] += normal.y;
			vertices[in0 + 2] += normal.z;
			vertices[in1] += normal.x;
			vertices[in1 + 1] += normal.y;
			vertices[in1 + 2] += normal.z;
			vertices[in2] += normal.x;
			vertices[in2 + 1] += normal.y;
			vertices[in2 + 2] += normal.z;
		}

		// divide by num of normals | same as normalizing
		for (size_t i = 0; i < vertices_count / vertex_length; i++)
		{
			unsigned int offset = (unsigned int)i * vertex_length + normal_offset;
			glm::vec3 n(vertices[offset], vertices[offset + 1], vertices[offset + 2]);
			n = glm::normalize(n);
			vertices[offset] = n.x;
			vertices[offset + 1] = n.y;
			vertices[offset + 2] = n.z;
		}
	}
}