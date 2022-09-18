#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include "mesh.h"

class Model
{
public:
	glm::vec3 pos;
	glm::vec3 size;
	std::vector<Mesh> meshes;
	std::string directory;

	Model();
	Model(glm::vec3 pos, glm::vec3 size);

	virtual void init();
	virtual void render(Shader shader);
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
	void cleanup();

private:
	std::vector<Texture> textures_loaded;

};