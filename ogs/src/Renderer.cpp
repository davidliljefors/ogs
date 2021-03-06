#include "ogspch.h"
#include "Renderer.h"

ogs::Shader const* current_shader = nullptr;

void ogs::Renderer::BeginScene(Camera const& camera)
{
	assert(current_shader && "No shader bound");
	current_shader->Bind();
	current_shader->SetMat4("u_VP", camera.GetVP());
}

void ogs::Renderer::UseShader(Shader const& shader)
{
	current_shader = &shader;
}

void ogs::Renderer::Draw(VertexArray const& vao)
{
	vao.Bind();
	if (vao.IsIndexed())
	{
		glDrawElements(GL_TRIANGLES, vao.Count(), GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vao.Count());
	}
}

void ogs::Renderer::Draw(Mesh const& mesh)
{
	Renderer::Draw(mesh._vao);
}

void ogs::Renderer::Draw(Mesh const& mesh, glm::mat4 const& model)
{
	current_shader->SetMat4("u_Model", model);
	Renderer::Draw(mesh._vao);
}

void ogs::Renderer::Draw(Mesh const& mesh, Material const& material, glm::mat4 const& model)
{
	material.Bind(*current_shader);
	Draw(mesh, model);
}
