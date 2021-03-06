#include "Model.h"
#include <exception>
#include <filesystem>
#include <fmt/core.h>
#include <iostream>

Model::Model(const char *path)
{
	_load_model(path);
}

void Model::draw(std::unique_ptr<ShaderProgram> &shader)
{
	for (auto const &mesh : _meshes)
	{
		mesh->draw(shader);
	}
}

std::vector<std::shared_ptr<Texture>> Model::_load_material_textures(aiMaterial *mat, aiTextureType ai_type, TextureMaterial type)
{

	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); i++)
	{
		aiString str;
		mat->GetTexture(ai_type, i, &str);
		auto path = std::filesystem::path(str.C_Str());
		auto filename = path.is_absolute() ? path.filename().string() : path.string();

		std::cout << filename << std::endl;

		bool skip = false;

		for (unsigned int j = 0; j < _textures_loaded.size(); i++)
		{
			if (std::strcmp(_textures_loaded[j]->file_name().data(), filename.data()) == 0)
			{
				textures.push_back(_textures_loaded[i]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			auto texture = std::make_shared<Texture>(_directory, filename, type);
			textures.push_back(texture);
		}
	}

	return textures;
}

std::unique_ptr<Mesh> Model::_process_mesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	// process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 v = glm::vec3(0.0f);
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;
		vertex.position = v;

		v.x = mesh->mNormals[i].x;
		v.y = mesh->mNormals[i].y;
		v.z = mesh->mNormals[i].z;
		vertex.normal = v;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 v = glm::vec2(0.0f);
			v.x = mesh->mTextureCoords[0][i].x;
			v.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_cords = v;
		}
		else
		{
			vertex.tex_cords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// process textures
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::shared_ptr<Texture>> diffuse_maps = _load_material_textures(material, aiTextureType_DIFFUSE, TextureMaterial::DIFFUSE);
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<std::shared_ptr<Texture>> specular_maps = _load_material_textures(material, aiTextureType_SPECULAR, TextureMaterial::SPECULAR);
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}
	else
	{
		throw std::runtime_error("Empty textures");
	}

	return std::make_unique<Mesh>(vertices, indices, textures);
}

void Model::_process_node(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_process_mesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		_process_node(node->mChildren[i], scene);
	}
}

void Model::_load_model(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::runtime_error(fmt::format("Failed to load aiScene: {}", importer.GetErrorString()));
	}
	_directory = path.substr(0, path.find_last_of('/'));
	_process_node(scene->mRootNode, scene);
}

Model::~Model()
{
}
