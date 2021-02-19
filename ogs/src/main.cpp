#include "GLContext.h"
#include "Renderer.h"

#include "imgui.h"

#include "temp/Mesh.h"
#include "Transform.h"


class Game : public ogs::GLContext
{
private:
	void OnConstruct() override
	{
		std::vector const quad_vertex_data = {
		-0.5F,  0.5F,  0.0F,  1.0F,
		 0.5F,  0.5F,  1.0F,  1.0F,
		 0.5F, -0.5F,  1.0F,  0.0F,
		-0.5F, -0.5F,  0.0F,  0.0F,
		};

		std::vector const quad_idx_data = {
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

		if (auto const obj_file = wavefront_load("res/meshes/Cube.obj"))
		{
			auto cube = std::make_unique<ogs::Mesh>(obj_file.value());
			meshes.emplace_back(ogs::Transform(), std::move(cube));
		}

		if (auto const obj_file = wavefront_load("res/meshes/Monkey.obj"))
		{
			auto monke = std::make_unique<ogs::Mesh>(obj_file.value());
			meshes.emplace_back(ogs::Transform(), std::move(monke));
		}

	}

	void OnUpdate(float) override
	{

		ImGui::Begin("Editor");

		static std::array<char, 128> filename_buffer;
		if( ImGui::InputText("Load Obj", filename_buffer.data(), filename_buffer.size(), ImGuiInputTextFlags_EnterReturnsTrue) )
		{
			ogs::LogHint("Loading obj file '{}'", filename_buffer.data());
			if (auto const obj_file = wavefront_load(filename_buffer.data()))
			{
				auto monke = std::make_unique<ogs::Mesh>(obj_file.value());
				meshes.emplace_back(ogs::Transform(), std::move(monke));
			}
		}
		ImGui::End();

		ImGui::BeginChild("Objects");
		if (ImGui::TreeNode("Scene"))
		{
			ImGui::Indent();
			for (int i = 0; i < meshes.size(); ++i)
			{
				if (ImGui::TreeNode(fmt::format("Object {} transform", i).c_str()))
				{
					if (ImGui::DragFloat3("Position", glm::value_ptr(meshes[i].first.Position), 0.1F)) { meshes[i].first.Invalidate(); }
					if (ImGui::DragFloat3("Rotation", glm::value_ptr(meshes[i].first.Rotation), 0.5F)) { meshes[i].first.Invalidate(); }
					if (ImGui::DragFloat3("Scale", glm::value_ptr(meshes[i].first.Scale), 0.1F)) { meshes[i].first.Invalidate(); }
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::EndChild();


		default_shader->Bind();
		default_shader->SetMat4("u_VP", _camera.GetVP());
		default_shader->SetFloat("u_Time", GetTime());

		for (auto const& mesh : meshes)
		{
			default_shader->SetMat4("u_Model", mesh.first.Mat4());
			ogs::Renderer::Submit(*mesh.second);
		}
	};

private:
	std::unique_ptr<ogs::VertexArray> quad_vao;
	std::unique_ptr<ogs::Shader> default_shader;
	std::unique_ptr<ogs::Texture> tex1;
	std::unique_ptr<ogs::Texture> tex2;

	std::vector<std::pair<ogs::Transform, std::unique_ptr<ogs::Mesh>>> meshes;
};

int main()
{
	Game game;
	game.Construct({ 1600, 900 });
	game.Run();

	return 0;
}
