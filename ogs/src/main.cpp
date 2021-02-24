#include "ogspch.h"
#include "GLContext.h"
#include "Renderer.h"

#include "imgui.h"

#include "temp/Mesh.h"
#include "Transform.h"

struct Object
{
	ogs::Mesh* mesh;
	ogs::Transform transform;
	ogs::Material material;
	std::string name = "Object";
};

struct Light
{
	ogs::Transform transform;
	glm::vec3 ambient  = glm::vec3(0.03F);
	glm::vec3 diffuse  = glm::vec3(1.0F);
	glm::vec3 specular = glm::vec3(0.5F);

	ogs::Mesh* mesh = nullptr;
	ogs::Material material{};
};

class Game : public ogs::GLContext
{
private:
	void OnConstruct() override
	{
		ogs::Material mat1(assets.GetTexture("res/textures/T_Box_D.png"), assets.GetTexture("res/textures/T_Box_S.png"));

		light = std::make_unique<Light>();
		light->mesh = assets.GetMesh("res/meshes/smoothsphere.obj");
		light->transform.SetScale({0.15F, 0.15F, 0.15F });
		light->transform.SetPosition({ 1.5F, 0.0F, 2.0F });

		assets.GetMeshAsync("res/meshes/untitled.obj", [mat=mat1, &objects = objects](ogs::Mesh* mesh)
			{
				objects.emplace_back(mesh, ogs::Transform(), mat, "Cube");
			});

		assets.GetMeshAsync("res/meshes/cube.obj", [mat = mat1, &objects = objects](ogs::Mesh* mesh)
			{
				ogs::Transform t;
				t.SetScale({ 10.0F, 0.1F, 10.0F });
				t.SetPosition({-5.0F, -1.0F, -5.0F });
				objects.emplace_back(mesh, t, mat, "Floor");
			});

		assets.GetMeshAsync("res/meshes/sphere.obj", [mat= mat1, &objects = objects](ogs::Mesh* mesh)
			{
				objects.emplace_back(mesh, ogs::Transform({3.0F, 0.0F, 0.0F}), mat, "Sphere");
			});

		assets.GetMeshAsync("res/meshes/smoothsphere.obj", [mat = mat1, &objects = objects](ogs::Mesh* mesh)
			{
				objects.emplace_back(mesh, ogs::Transform({ 9.0F, 0.0F, 0.0F }), mat, "Smooth Sphere");
			});

		assets.GetMeshAsync("res/meshes/monkey.obj", [mat= mat1, &objects = objects](ogs::Mesh* mesh)
			{
				objects.emplace_back(mesh, ogs::Transform({ 6.0F, 0.0F, 0.0F }), mat, "Monke");
			});

		shader = std::make_unique<ogs::Shader>("res/shaders/lit.vert", "res/shaders/lit.frag");
		ogs::Renderer::UseShader(*shader);
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
					objects.emplace_back(mesh, ogs::Transform(), ogs::Material());
				});
		}

		if (ImGui::DragFloat3("Light position", light->transform.PosPtr(), 0.01F)) { light->transform.Invalidate(); }
		ImGui::ColorEdit3("Light Diffuse", glm::value_ptr(light->diffuse));

		ImGui::End();
		
		ImGui::BeginChild("Objects");
		if (ImGui::TreeNode("Scene"))
		{
			ImGui::Indent();
			for (int i = 0; auto& object : objects)
			{
					if (ImGui::TreeNode(object.name.c_str()))
					{
						if (ImGui::DragFloat3("Position", object.transform.PosPtr()), 0.1F) { object.transform.Invalidate(); }
						if (ImGui::DragFloat3("Rotation", object.transform.RotPtr()), 0.5F) { object.transform.Invalidate(); }
						if (ImGui::DragFloat3("Scale", object.transform.ScalePtr()), 0.01F) { object.transform.Invalidate(); }
						ImGui::TreePop();
					}
					++i;
				}
				ImGui::TreePop();
			}
		ImGui::EndChild();

		shader->SetFloat3("u_EyePos", _camera.GetPosition());

		shader->SetFloat3("u_Light.position", light->transform.GetPosition());
		shader->SetFloat3("u_Light.ambient", light->ambient);
		shader->SetFloat3("u_Light.diffuse", light->diffuse);
		shader->SetFloat3("u_Light.specular", light->specular);
		ogs::Renderer::Draw(*light->mesh, light->material, light->transform.Mat4());

		for (auto const& object : objects)
		{
			ogs::Renderer::Draw(*object.mesh, object.material, object.transform.Mat4());
		}
	};

private:
	std::unique_ptr<Light> light;

	std::vector<Object> objects;
	std::unique_ptr<Object> point_light;
	std::unique_ptr<ogs::Shader> shader;
};

int main()
{
	Game game;
	game.Construct({ 1600, 900 });
	game.Run();
	return 0;
}
