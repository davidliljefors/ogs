#include "Renderer.h"

void ogs::Renderer::Submit(VertexArray const& vao)
{
	vao.Bind();
	glDrawElements(GL_TRIANGLES, vao.Count(), GL_UNSIGNED_INT, nullptr);
}
