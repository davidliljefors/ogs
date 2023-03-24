#include "ogspch.h"
#include <stb_image.h>

#include "Texture.h"

std::unique_ptr<ogs::Texture> ogs::Texture::s_pWhiteTexture;

ogs::Texture const *ogs::Texture::GetWhiteTexture()
{
	if (!s_pWhiteTexture)
	{
		s_pWhiteTexture = std::unique_ptr<Texture>(new Texture());
	}
	return s_pWhiteTexture.get();
}

ogs::Texture::Texture()
	: m_id(GenGL(glGenTextures))
{
	constexpr int texture_size = 1 * 1;
	m_height = 1;
	m_width = 1;
	m_channels = 4;

	auto tex_data = std::make_unique<glm::u8vec4[]>(texture_size);
	glm::u8vec4 white = {255, 255, 255, 255};
	for (ptrdiff_t i = 0; i < texture_size; ++i)
	{
		tex_data[i] = white;
	}

	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data.get());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

ogs::Texture::Texture(std::string const &texture_path)
	: m_id(GenGL(glGenTextures))
{
	stbi_set_flip_vertically_on_load(1);
	auto image_data = stbi_load(texture_path.c_str(), &m_width, &m_height, &m_channels, 4);

	assert(image_data && "Failed to load image");

	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(image_data);
}

ogs::Texture::Texture(Texture &&texture) noexcept
{
	std::swap(m_id, texture.m_id);
	m_width = texture.m_width;
	m_height = texture.m_height;
	m_channels = texture.m_channels;
}

ogs::Texture &ogs::Texture::operator=(Texture &&rhs) noexcept
{
	std::swap(m_id, rhs.m_id);
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_channels = rhs.m_channels;
	return *this;
}

ogs::Texture::~Texture()
{
	if (m_id)
	{
		glDeleteTextures(1, &m_id);
	}
}

void ogs::Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}
