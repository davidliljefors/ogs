#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <utility>

class GLContext
{
public:

	GLContext()
	{
		glfwInit();
		_window = glfwCreateWindow(1280, 720, "ogs", NULL, NULL);

		if (!_window)
		{
			glfwTerminate();
			std::cerr << "GLFW : Create Window Error" << std::endl;
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
		while (!glfwWindowShouldClose(_window))
		{
			glfwPollEvents();
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
