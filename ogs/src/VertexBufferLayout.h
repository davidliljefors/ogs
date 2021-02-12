#pragma once
#include <glad/glad.h>
#include <vector>
#include <cassert>

namespace ogs {

struct VertexBufferElement {

	unsigned int type = 0;
	unsigned int count = 0;
	unsigned char normalized = 0;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		}
		assert(false && "Invalid GL Type");
		return 0;
	}
};

class VertexBufferLayout {

public:
	VertexBufferLayout() {}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false, "Invalid Argument Type");
	}

	template<>
	void Push<float>(unsigned int count)
	{
		_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return _elements; }
	inline unsigned int GetStride() const { return _stride; }

private:
	std::vector<VertexBufferElement> _elements;
	unsigned int _stride = 0;

};
}