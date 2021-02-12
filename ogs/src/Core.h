#pragma once

#include <cassert>

// Wrapper to enable keeping OpenGL pointer as const
template<typename Func>
inline auto GenGL(Func gl_func)
{
	unsigned int glptr;
	gl_func(1, &glptr);
	return glptr;
};