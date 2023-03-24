#pragma once
#include <vector>

#include "Core.h"
#include "CoreTypes.h"
#include "VertexBufferLayout.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ogs
{

	struct Vertex
	{
		glm::vec3 Position{};
		glm::vec2 TexCoord{};
		glm::vec3 Normal{};
	};

	class VertexArray
	{

	public:
		VertexArray() = default;
		VertexArray(std::vector<f32> const &vertex_data,
					std::vector<i32> const &index_data,
					VertexBufferLayout const &layout);

		VertexArray(std::vector<Vertex> const &vertex_data,
					VertexBufferLayout const &layout);

		VertexArray(VertexArray const &) = delete;
		VertexArray &operator=(VertexArray const &) = delete;

		VertexArray(VertexArray &&vao) noexcept
		{
			std::swap(m_id, vao.m_id);
			m_count = vao.m_count;
		}

		VertexArray &operator=(VertexArray &&rhs) noexcept
		{
			std::swap(m_id, rhs.m_id);
			m_count = rhs.m_count;
			return *this;
		}

		~VertexArray();

		auto Count() const
		{
			return m_count;
		}

		void Bind() const;

		auto IsIndexed() const { return _using_index_buffer; }

	private:
		bool _using_index_buffer = false;
		u32 m_id = 0;
		u32 m_count = 0;
	};
}
