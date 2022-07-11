#include "texture.h"

Texture::Texture() {}

Texture::Texture(std::string path, std::string name)
	:path(path), name(name)
{
	generate();
	load();
}

void Texture::generate()
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::load()
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	GLenum colorMode = GL_RGB;

	switch (nrChannels)
	{
	case 1:
		colorMode = GL_RED;
		break;
	case 4:
		colorMode = GL_RGBA;
		break;
	}

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture: " << path << std::endl;
	}

	stbi_image_free(data);
}

void Texture::active_bind()
{
	//TODO: need to check if better write manually GL_TEXTURE0, GL_TEXTURE1 etc or not
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, id);
}
