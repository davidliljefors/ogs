#pragma once

#include <string>

namespace ogs {

class Texture
{

public:
	Texture(std::string const& texture_path);

	Texture(Texture const&) = delete;
	Texture& operator=(Texture const&) = delete;

	Texture(Texture&& texture) noexcept;
	Texture& operator=(Texture&& rhs) noexcept;

	~Texture();

	void Bind(unsigned int slot) const;

	inline auto GetWidth() const { return m_width; };
	inline auto GetHeight() const { return m_height; };

	static Texture const* GetWhiteTexture();

private:
	Texture(); // default construct a white texture
	static std::unique_ptr<Texture> s_pWhiteTexture;

	unsigned int m_id = 0;
	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;
};

}