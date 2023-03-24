#include "ogspch.h"
#include "GLContext.h"
#include "Renderer.h"

#include "imgui.h"

#include "temp/Mesh.h"
#include "Transform.h"

#include "core-lib/Hash.h"

struct Object
{
	ogs::MeshResource* mesh;
	ogs::Transform transform;
	ogs::Material material;
	std::string name = "Object";
};

struct Light
{
	ogs::Transform transform;
	glm::vec3 ambient = glm::vec3(0.3F);
	glm::vec3 diffuse = glm::vec3(1.0F);
	glm::vec3 specular = glm::vec3(0.5F);

	ogs::MeshResource* mesh = nullptr;
	ogs::Material material;
};

std::array<float, 10> history;
auto updateWindowTitle = [count = 0](GLFWwindow* pWindow, float dt) mutable {
	history[count] = dt;
	if (++count == history.size())
	{
		count = 0;
		auto avg = std::reduce(history.begin(), history.end(), 0.0f) / float(history.size());
		glfwSetWindowTitle(pWindow, fmt::format("FPS - {:.2f}", 1.0f / avg).c_str());
	}
};

class Game : public ogs::GLContext
{
private:
	void OnConstruct() override
	{
		ogs::Material mat1(assets.GetTexture("res/textures/T_Box_D.png"), assets.GetTexture("res/textures/T_Box_S.png"));

		light = std::make_unique<Light>();
		light->mesh = assets.GetMesh("res/meshes/smoothsphere.obj");
		light->transform.SetScale({ 0.15F, 0.15F, 0.15F });
		light->transform.SetPosition({ 1.5F, 0.0F, 2.0F });


		{
			//floor 
			auto mesh = assets.GetMesh("res/meshes/cube.obj");
			ogs::Transform t;
			t.SetScale({ 10.0F, 0.1F, 10.0F });
			t.SetPosition({ -5.0F, -1.0F, -5.0F });
			objects.emplace_back(mesh, t, mat1, "Floor");
		}

		for (int i = -10; i < 11; ++i)
		{
			for (int j = -10; j < 11; ++j)
			{
				auto mesh = assets.GetMesh(rand() % 2 == 0 ? "res/meshes/cube.obj" : "res/meshes/Monkey.obj");
				ogs::Transform t;
				t.SetPosition({ -2.0F * f32(i), -1.0F, -2.0F * f32(j) });
				objects.emplace_back(mesh, t, mat1, "Monke");
			}
		}

		std::sort(objects.begin(), objects.end(), [](Object const& l, Object const& r) {
			return l.mesh < r.mesh;
		});

		shader = std::make_unique<ogs::Shader>("res/shaders/lit.vert", "res/shaders/lit.frag");
		ogs::Renderer::UseShader(*shader);
	}

	void OnUpdate(float dt) override
	{
		ImGui::Begin("Editor");
		
		updateWindowTitle(m_pWindow, dt);

		if (ImGui::DragFloat3("Light position", light->transform.PosPtr(), 0.01F)) { light->transform.Invalidate(); }
		ImGui::ColorEdit3("Light Diffuse", glm::value_ptr(light->diffuse));

		ImGui::End();

		ImGui::BeginChild("Objects");
		if (ImGui::TreeNode("Scene"))
		{
			ImGui::Indent();
			for (int i = 0; auto & object : objects)
			{
				ImGui::PushID(i);
				if (ImGui::TreeNode(object.name.c_str()))
				{
					if (ImGui::DragFloat3("Position", object.transform.PosPtr(), 0.1F))
					{
						object.transform.Invalidate();
					}
					auto mat4 = object.transform.Mat4();

					if (ImGui::DragFloat3("Rotation", object.transform.RotPtr(), 0.5F))
					{
						object.transform.Invalidate();
					}

					if (ImGui::DragFloat3("Scale", object.transform.ScalePtr(), 0.01F))
					{
						object.transform.Invalidate();
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
				++i;
			}
			ImGui::TreePop();
		}
		ImGui::EndChild();

		shader->SetFloat3("u_EyePos", m_camera.GetPosition());

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
