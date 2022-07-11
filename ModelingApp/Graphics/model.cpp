#include "model.h"

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

void Model::render(Shader shader)
{

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::scale(model, size);

	shader.setMat4("model", model);


	for (Mesh mesh : meshes)
	{
		mesh.render(shader);
	}
}