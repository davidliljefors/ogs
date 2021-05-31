#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Core.h"
#include "VertexBufferLayout.h"

namespace ogs {

struct Vertex
{
	glm::vec3 Position{};
	glm::vec2 TexCoord{};
	glm::vec3 Normal{};
};

class VertexArray {

public:
	VertexArray() = default;
	VertexArray( std::vector<float> const& vertex_data,
		std::vector<int>   const& index_data,
		VertexBufferLayout const& layout );

	VertexArray( std::vector<Vertex> const& vertex_data,
		VertexBufferLayout const& layout );


	VertexArray( VertexArray const& ) = delete;
	VertexArray& operator=( VertexArray const& ) = delete;

	VertexArray( VertexArray&& vao ) noexcept
	{
		std::swap( _id, vao._id );
		_count = vao._count;
	}

	VertexArray& operator=( VertexArray&& rhs ) noexcept
	{
		std::swap( _id, rhs._id );
		_count = rhs._count;
		return *this;
	}

	~VertexArray();

	auto Count() const
	{
		return _count;
	}

	void Bind() const;

	auto IsIndexed() const { return _using_index_buffer; }

private:
	bool _using_index_buffer = false;
	unsigned int _id = 0;
	unsigned int _count = 0;
};
}
