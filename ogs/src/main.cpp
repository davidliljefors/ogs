#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <utility>
#include <array>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"


class GLContext
{
public:

	GLContext()
	{
		glfwInit();
		_window = glfwCreateWindow(800, 800, "ogs", NULL, NULL);

		if (!_window)
		{
			glfwTerminate();
			std::cerr << "GLFW : Create Window Error" << std::endl;
			std::exit(-1);
		}

		glfwMakeContextCurrent(_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "GLAD : Failed to initialize GLAD" << std::endl;
			std::exit(-1);
		}

		auto ResizeCallback = [](GLFWwindow*, int xsize, int ysize)
		{
			glViewport(0, 0, xsize, ysize);
		};

		glfwSetWindowSizeCallback(_window, ResizeCallback);

		auto KeypressCallback = [](GLFWwindow* window, int key, int, int action, int mods)
		{

			if (action != GLFW_PRESS)
			{
				return;
			}

			if (key == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}

			if (key == GLFW_KEY_ENTER &&
				(mods & GLFW_MOD_ALT))
			{
				auto const maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);

				if (maximized == GLFW_TRUE)
				{
					glfwRestoreWindow(window);
				}
				else
				{
					glfwMaximizeWindow(window);
				}
			}

			auto GetWindowPos = [](auto window) {
				int x, y;
				glfwGetWindowPos(window, &x, &y);
				return std::make_pair(x, y);
			};

			if (key == GLFW_KEY_UP)
			{
				auto const windowPos = GetWindowPos(window);
				glfwSetWindowPos(window, windowPos.first, windowPos.second - 10);
			}

			if (key == GLFW_KEY_DOWN)
			{
				auto const windowPos = GetWindowPos(window);
				glfwSetWindowPos(window, windowPos.first, windowPos.second + 10);
			}

			if (key == GLFW_KEY_RIGHT)
			{
				auto const windowPos = GetWindowPos(window);
				glfwSetWindowPos(window, windowPos.first + 10, windowPos.second);
			}

			if (key == GLFW_KEY_LEFT)
			{
				auto const windowPos = GetWindowPos(window);
				glfwSetWindowPos(window, windowPos.first - 10, windowPos.second);
			}
		};

		glfwSetKeyCallback(_window, KeypressCallback);

		auto MouseButtonCallback = [](GLFWwindow* window, int button, int action, int)
		{
			if (action != GLFW_PRESS)
			{
				return;
			}

			if (button == GLFW_MOUSE_BUTTON_1)
			{
				auto const mousePos = [&]() {
					double x, y;
					glfwGetCursorPos(window, &x, &y);
					return std::make_pair(x, y);
				}();

				std::cout << "bang!\n" << "mouse pos = {" << mousePos.first << ", " << mousePos.second << "}\n";
			}
		};

		glfwSetMouseButtonCallback(_window, MouseButtonCallback);

		auto ScrollCallback = [](GLFWwindow*, double xoffset, double yoffset)
		{
			std::cout << "Scrolled x: " << xoffset << ", y: " << yoffset << ".\n";
		};

		glfwSetScrollCallback(_window, ScrollCallback);
	}

	~GLContext()
	{
		glfwTerminate();
	}

	void Run()
	{
		std::array const vertex_data = {
			-0.5F,  0.5F,  0.0f,  1.0f,
			 0.5F,  0.5F,  1.0f,  1.0f,
			 0.5F, -0.5F,  1.0f,  0.0f,
			-0.5F, -0.5F,  0.0f,  0.0f,
		};

		std::array const vertex_data2 = {
			-0.5F,  0.5F,  1.0f,  0.0f,
			 0.5F,  0.5F,  0.0f,  0.0f,
			 0.5F, -0.5F,  0.0f,  1.0f,
			-0.5F, -0.5F,  1.0f,  1.0f,
		};

		std::array const index_data = {
			0, 1, 2,
			0, 2, 3,
		};

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		VertexArray vao(vertex_data, index_data, layout);
		VertexArray vao2(vertex_data2, index_data, layout);

		vao.Bind();

		Shader test_shader("res/shaders/color.vert", "res/shaders/color.frag");
		test_shader.Bind();

		test_shader.SetFloat4("u_Color", glm::vec4(0.2f, 0.8f, 0.4f, 1.0f));

		while (!glfwWindowShouldClose(_window))
		{
			glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
			glClear(GL_COLOR_BUFFER_BIT);

			vao.Bind();
			glm::mat4 model = glm::mat4(1.0F);
			model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
			test_shader.SetMat4("u_Model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			vao2.Bind();
			model = glm::translate(model, glm::vec3(1.f, 0.0f, 0.0f));
			test_shader.SetMat4("u_Model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glfwPollEvents();
			glfwSwapBuffers(_window);
		}
	}

private:
	GLFWwindow* _window = nullptr;
};

int main()
{
	GLContext ctx;
	ctx.Run();

	return 0;
}
