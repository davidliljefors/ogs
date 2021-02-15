#include "Texture.h"
#include "Core.h"


ogs::Texture::Texture(std::string const& texture_path)
	:_id(GenGL(glGenTextures))
{
	stbi_set_flip_vertically_on_load(1);
	auto image_data = stbi_load(texture_path.c_str(), &_width, &_height, &_channels, 4);
		
	assert(image_data && "Failed to load image");

	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(image_data);
}

ogs::Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

void ogs::Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _id);
}

