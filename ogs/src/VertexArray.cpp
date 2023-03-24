#include "ogspch.h"
#include "VertexArray.h"

ogs::VertexArray::VertexArray(std::vector<float> const& vertex_data,
	std::vector<int> const& index_data,
	VertexBufferLayout const& layout)
	: m_count(static_cast<unsigned int>(index_data.size()))
{
	auto const buffer = GenGL(glGenBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);

	{
		auto const& elements = layout.GetElements();
		std::ptrdiff_t offset = 0;
		for (auto i = 0; i < elements.size(); ++i)
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * index_data.size(), index_data.data(), GL_STATIC_DRAW);

	glBindVertexArray(m_id);
	_using_index_buffer = true;
}

ogs::VertexArray::VertexArray(std::vector<Vertex> const& vertex_data,
	VertexBufferLayout const& layout)
	: m_count(static_cast<unsigned int>(vertex_data.size()))
{

	auto const buffer = GenGL(glGenBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);

	{
		auto const& elements = layout.GetElements();
		std::ptrdiff_t offset = 0;
		for (auto i = 0; i < elements.size(); ++i)
		{
			auto const& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type,
				element.normalized, layout.GetStride(), (const void*)offset);

			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
	}

	glBindVertexArray(m_id);
	_using_index_buffer = false;
}

ogs::VertexArray::~VertexArray()
{
	if (m_id)
	{
		glDeleteVertexArrays(1, &m_id);
	}
}

void ogs::VertexArray::Bind() const
{
	glBindVertexArray(m_id);
}
