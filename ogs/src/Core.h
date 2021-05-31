#pragma once

#include <cassert>
#include <fmt/core.h>
#include <fmt/color.h>

namespace ogs {

template<typename...Ts>
void LogError( Ts... ts )
{
	fmt::print( fg( fmt::color::light_coral ), ts... );
	fmt::print( "\n" );
}

template<typename...Ts>
void LogWarning( Ts... ts )
{
	fmt::print( fg( fmt::color::light_yellow ), ts... );
	fmt::print( "\n" );
}

template<typename...Ts>
void LogHint( Ts... ts )
{
	fmt::print( fg( fmt::color::light_green ), ts... );
	fmt::print( "\n" );
}

// Wrapper to enable keeping OpenGL pointer as const
template<typename Func>
inline auto GenGL( Func gl_func )
{
	unsigned int glptr;
	gl_func( 1, &glptr );
	return glptr;
};

}