#include "ogspch.h"
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
		for (int j = 0; j < 10; ++j)
		{
			for (int i = 0; i < 10; ++i)
			{
				assets.GetMeshAsync("res/meshes/sphere.obj", [&meshes = meshes, i=i, j=j](ogs::Mesh* mesh)
					{
						meshes.emplace_back(glm::vec3(i * 3.0F, 0.0F, j * 3.0F), mesh);
					});

			}
		}
	}

	void OnUpdate(float) override
	{
		ImGui::Begin("Editor");

		static std::array<char, 128> filename_buffer;
		if( ImGui::InputText("Load Obj", filename_buffer.data(), filename_buffer.size(), ImGuiInputTextFlags_EnterReturnsTrue) )
		{
			ogs::LogHint("Loading obj file '{}'", filename_buffer.data());
			assets.GetMeshAsync(filename_buffer.data(), [&](ogs::Mesh* mesh)
				{
					meshes.emplace_back(ogs::Transform(), mesh);
				});
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
					if (ImGui::DragFloat3("Scale", glm::value_ptr(meshes[i].first.Scale), 0.01F)) { meshes[i].first.Invalidate(); }
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::EndChild();

		for (auto const& mesh : meshes)
		{
			ogs::Renderer::Draw(*mesh.second, mesh.first.Mat4());
		}
	};

private:
	std::vector<std::pair<ogs::Transform, ogs::Mesh*>> meshes;
};

int main()
{
	Game game;
	game.Construct({ 1600, 900 });
	game.Run();
	return 0;
}
