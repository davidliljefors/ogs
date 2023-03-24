#pragma once

#include "CoreTypes.h"

template<typename T>
class Span
{
public:
	Span(T* begin, T* end)
		: m_begin(begin)
		, m_size(static_cast<u64>(end - m_begin))
	{

	}

	u64 size() const { return m_size; }

	T const* begin() const { return m_begin; }
	T const* end() const { return m_begin + m_size; }

	T* begin() { return m_begin; }
	T* end() { return m_begin + m_size; }

private:
	T* m_begin = nulltptr;
	u64 m_size = 0;
};