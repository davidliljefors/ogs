#include "GLContext.h"
#include "Renderer.h"

#include "imgui.h"


class Game : public ogs::GLContext
{
private:
	void OnConstruct() override
	{
		std::array const quad_vertex_data = {
		-0.5F,  0.5F,  0.0F,  1.0F,
		 0.5F,  0.5F,  1.0F,  1.0F,
		 0.5F, -0.5F,  1.0F,  0.0F,
		-0.5F, -0.5F,  0.0F,  0.0F,
		};

		std::array const quad_idx_data = {
			0, 1, 2,
			0, 2, 3,
		};

		ogs::VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		quad_vao = std::make_unique<ogs::VertexArray>(quad_vertex_data, quad_idx_data, layout);
		default_shader = std::make_unique<ogs::Shader>("res/shaders/color.vert", "res/shaders/color.frag");

		tex1 = std::make_unique<ogs::Texture>("res/textures/very_nice_texture.png");
		tex2 = std::make_unique<ogs::Texture>("res/textures/cool_texture.png");
		tex1->Bind(0);
		tex2->Bind(1);

		default_shader->Bind();
		default_shader->SetInt("u_Tex0", 0);
		default_shader->SetInt("u_Tex1", 1);
	}

	void OnUpdate(float) override
	{

		ImGui::Begin("Editor");
		ImGui::DragFloat3("Offset position", glm::value_ptr(offset));
		ImGui::ColorEdit4("Tint", glm::value_ptr(tint));
		ImGui::End();

		default_shader->Bind();
		default_shader->SetMat4("u_VP", _camera.GetVP());
		default_shader->SetFloat("u_Time", GetTime());
		default_shader->SetFloat4("u_Color", tint);

		for (int y = -10; y < 10; ++y)
		{
			for (int x = -10; x < 10; ++x)
			{
				auto const pos = offset + glm::vec3(static_cast<float>(x), static_cast<float>(y), 0.0F);

				auto model = glm::translate(glm::mat4(1.0F), pos);
				model = glm::scale(model, glm::vec3(1.5F));
				model = model * model;
				model = glm::rotate(model, x / 5.0F + GetTime() , glm::vec3{ 0.0F, 1.0F, 0.0F });
				default_shader->SetMat4("u_Model", model);

				ogs::Renderer::Submit(*quad_vao);
			}
		}
	};
private:
	glm::vec3 offset{};
	glm::vec4 tint = glm::vec4(1.0F);

	std::unique_ptr<ogs::VertexArray> quad_vao;
	std::unique_ptr<ogs::Shader> default_shader;
	std::unique_ptr<ogs::Texture> tex1;
	std::unique_ptr<ogs::Texture> tex2;
};

int main()
{
	Game game;
	game.Construct({ 1600, 900 });
	game.Run();

	return 0;
}
