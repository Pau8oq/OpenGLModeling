#include "model.h"
#include "material.h"

Model::Model()
{
	this->pos = glm::vec3(0.0f);
	this->size = glm::vec3(1.0f);
}

Model::Model(glm::vec3 pos, glm::vec3 size)
	: pos(pos), size(size) {}

void Model::init() {}

void Model::cleanup()
{
	for (Mesh mesh : meshes)
	{
		mesh.cleanup();
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z);

		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0])
		{
			vertex.texture = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else
			vertex.texture = glm::vec2(0.0f);


		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::cout << str.C_Str() << std::endl;

		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			std::string full_path = (directory + "/" + str.C_Str()).c_str();
			Texture texture(full_path, str.C_Str(), type);
			texture.load();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

void Model::render(Shader shader)
{

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::scale(model, size);

	shader.setMat4("model", model);
	shader.setFloat("material.shininess", 0.2f);

	for (Mesh mesh : meshes)
	{
		mesh.render(shader);
	}
}