#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <utility>
#include <array>

class GLContext
{
public:

	GLContext()
	{
		glfwInit();
		_window = glfwCreateWindow(800, 600, "ogs", NULL, NULL);

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

		auto KeypressCallback = [](GLFWwindow* window, int key, int scandcode, int action, int mods) 
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
				glfwSetWindowPos(window, windowPos.first, windowPos.second-10);
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

		auto MouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods)
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

				std::cout << "bang!\n" << "mouse pos = {" << mousePos.first << ", " << mousePos.second <<"}\n";
			}
		};

		glfwSetMouseButtonCallback(_window, MouseButtonCallback);

		auto ScrollCallback = [](GLFWwindow* window, double xoffset, double yoffset)
		{
			std::cout << "Scrolled x: " << xoffset << ", y: " << yoffset << ".\n";
		};

		glfwSetScrollCallback(_window, ScrollCallback);
	}

	void Run()
	{
		std::array const vertex_data = {
			 0.0F,  0.7F,
			-0.7F, -0.7F,
			 0.7F, -0.7F,
		};

		GLuint const triangle_buffer = []() {
			GLuint val;
			glGenBuffers(1, &val);
			return val;
		}();

		glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data.data(), GL_STATIC_DRAW);

		GLuint const vao = []() {
			GLuint val;
			glGenVertexArrays(1, &val);
			return val;
		}();

		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

		while (!glfwWindowShouldClose(_window))
		{
			glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawArrays(GL_TRIANGLES, 0, 3);

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
