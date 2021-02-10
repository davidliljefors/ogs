#pragma once
#include <glad/glad.h>
#include <array>

#include "VertexBufferLayout.h"

class VertexArray
{
public:
	template<int vertex_count, int index_count>
	VertexArray(std::array<float, vertex_count> const& vertex_data, 
				std::array<int, index_count> const& index_data ,
				VertexBufferLayout const& layout)
		:_indices(index_count)
	{
		auto GenGL = [](auto gl_func)
		{
			GLuint glptr;
			gl_func(1, &glptr);
			return glptr;
		};

		auto const buffer = GenGL(glGenBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &_id);
		glBindVertexArray(_id);

		{
			auto const& elements = layout.GetElements();
			std::ptrdiff_t offset = 0;
			for (GLuint i = 0; i < elements.size(); ++i)
			{
				auto const& element = elements[i];
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, element.count, element.type,
					element.normalized, layout.GetStride(), (const void*)offset);

				offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
			}
		}

		auto const ebo = GenGL(glGenBuffers);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data.data(), GL_STATIC_DRAW);

		glBindVertexArray(_id);
	}

	~VertexArray()
	{
		glDeleteVertexArrays(1, &_id);
	}

	auto Count() const
	{
		return _indices;
	}

	void Bind() const
	{
		glBindVertexArray(_id);
	}

private:
	GLuint _id;
	GLuint _indices;
};