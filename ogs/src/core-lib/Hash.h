#pragma once

#include "CoreTypes.h"


u64 MurmurHash64A(void const* key, i32 len, u64 seed);

inline u64 Hash(char const* str, u64 seed = 0)
{
	const i32 size = static_cast<i32>(strlen(str));
	return MurmurHash64A(static_cast<void const*>(str), size, seed);
}


template<typename T>
u64 Hash(T const* pData, u64 seed = 0)
{
	constexpr i32 size = sizeof(T);
	return MurmurHash64A(static_cast<void const*>(pData), size, seed);
}
