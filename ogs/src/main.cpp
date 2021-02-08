#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>


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
	std::cout << "GLFW Window Creating done!" << std::endl;
	ctx.Run();

	return 0;
}
