#include "Renderer.h"

void ogs::Renderer::Submit(VertexArray const& vao)
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

void ogs::Renderer::Submit(Mesh const& mesh)
{
	Renderer::Submit(mesh._vao);
}
