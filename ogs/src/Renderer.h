#pragma once

#include "Camera.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "temp/Mesh.h"
#include "components/MeshComponent.h"

namespace ogs {

class Renderer
{
public:
	static void BeginScene(Camera const& camera);
	static void UseShader(Shader const& shader);

	static void Draw(VertexArray const& vao);
	static void Draw(MeshResource const& mesh);
	static void Draw(MeshResource const& mesh, glm::mat4 const& model);
	static void Draw(MeshResource const& mesh, Material const& material, glm::mat4 const& model);

	static void Draw(MeshManager const& mgr);
};

}