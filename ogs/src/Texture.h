#pragma once

#include <stb_image.h>
#include <glad/glad.h>

#include <string>

namespace ogs {

class Texture {

public:
	Texture(std::string const& texture_path);
	~Texture();

	void Bind(unsigned int slot) const;

	inline auto GetWidth() const { return _width;   };
	inline auto GetHeight() const { return _height; };

private:
	unsigned int const _id;
	int _width		= 0;
	int _height		= 0;
	int _channels	= 0;
};
}