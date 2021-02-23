#pragma once

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "temp/Mesh.h"

namespace ogs
{

class Renderer
{
public:
	static void BeginScene(Camera const& camera);
	static void UseShader(Shader const& shader);

	static void Draw(VertexArray const& vao);
	static void Draw(Mesh const& mesh);
	static void Draw(Mesh const& mesh, glm::mat4 const& model);
};

}