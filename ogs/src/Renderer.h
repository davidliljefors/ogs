#pragma once

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

namespace ogs
{

class Renderer
{
public:
	static void Submit(VertexArray const& vao);
};

}