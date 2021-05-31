#include "ogspch.h"
#include <stb_image.h>

#include "Texture.h"

std::unique_ptr<ogs::Texture> ogs::Texture::_white_texture;

ogs::Texture const* ogs::Texture::GetWhiteTexture()
{
	if ( !_white_texture )
	{
		_white_texture = std::unique_ptr<Texture>( new Texture() );
	}
	return _white_texture.get();
}

ogs::Texture::Texture()
	:_id( GenGL( glGenTextures ) )
{
	constexpr int texture_size = 1 * 1;
	_height = 1;
	_width = 1;
	_channels = 4;

	auto tex_data = std::make_unique<glm::u8vec4[]>( texture_size );
	glm::u8vec4 white = { 255, 255, 255, 255 };
	for ( ptrdiff_t i = 0; i < texture_size; ++i )
	{
		tex_data[i] = white;
	}

	glBindTexture( GL_TEXTURE_2D, _id );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data.get() );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

ogs::Texture::Texture( std::string const& texture_path )
	:_id( GenGL( glGenTextures ) )
{
	stbi_set_flip_vertically_on_load( 1 );
	auto image_data = stbi_load( texture_path.c_str(), &_width, &_height, &_channels, 4 );

	assert( image_data && "Failed to load image" );

	glBindTexture( GL_TEXTURE_2D, _id );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	stbi_image_free( image_data );
}

ogs::Texture::Texture( Texture&& texture ) noexcept
{
	std::swap( _id, texture._id );
	_width = texture._width;
	_height = texture._height;
	_channels = texture._channels;
}

ogs::Texture& ogs::Texture::operator=( Texture&& rhs ) noexcept
{
	std::swap( _id, rhs._id );
	_width = rhs._width;
	_height = rhs._height;
	_channels = rhs._channels;
	return *this;
}

ogs::Texture::~Texture()
{
	if ( _id )
	{
		glDeleteTextures( 1, &_id );
	}
}

void ogs::Texture::Bind( unsigned int slot ) const
{
	glActiveTexture( GL_TEXTURE0 + slot );
	glBindTexture( GL_TEXTURE_2D, _id );
}

