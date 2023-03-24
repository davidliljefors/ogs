#pragma once

#include <cassert>
#include <fmt/core.h>
#include <fmt/color.h>

#include "CoreTypes.h"

namespace ogs {

template<typename...Ts>
void LogError(Ts const&... ts)
{
	fmt::print(fg(fmt::color::light_coral), ts...);
	fmt::print("\n");
}

template<typename...Ts>
void LogWarning(Ts const&... ts)
{
	fmt::print(fg(fmt::color::light_yellow), ts...);
	fmt::print("\n");
}

template<typename...Ts>
void LogHint(Ts const&... ts)
{
	fmt::print(fg(fmt::color::light_green), ts...);
	fmt::print("\n");
}

template<typename Func>
inline auto GenGL(Func gl_func)
{
	unsigned int glptr;
	gl_func(1, &glptr);
	return glptr;
};

}