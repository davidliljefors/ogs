#pragma once

#include <string>

namespace ogs {
class Texture {

public:
	
	Texture(std::string const& texture_path);

	Texture(Texture const&) = delete;
	Texture& operator=(Texture const&) = delete;

	Texture(Texture&& texture) noexcept;
	Texture& operator=(Texture&& rhs) noexcept;

	~Texture();

	void Bind(unsigned int slot) const;

	inline auto GetWidth() const { return _width;   };
	inline auto GetHeight() const { return _height; };

	static Texture const* GetWhiteTexture();

private:
	Texture(); // default construct a black/magenta texture
	static std::unique_ptr<Texture> _white_texture;

	unsigned int _id = 0;
	int _width		 = 0;
	int _height		 = 0;
	int _channels	 = 0;
};
}