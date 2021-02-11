#include "GLContext.h"


GLContext::GLContext()
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
	glfwSetWindowUserPointer(_window, &_input);

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
		auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));
		assert(input && "Window User Pointer should not be null!");

		input->KeyEvent(key, action);

		if (action != GLFW_PRESS)
		{
			return;
		}

		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			return;
		}

		if (key == GLFW_KEY_ENTER &&
			(mods & GLFW_MOD_ALT))
		{
			auto const maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);

			if (maximized == GLFW_TRUE)
			{
				glfwRestoreWindow(window);
				return;
			}
			else
			{
				glfwMaximizeWindow(window);
				return;
			}
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

GLContext::~GLContext()
{
	glfwTerminate();
}

void GLContext::Run()
{
	std::array const tetramino_square_data = {
		-1.0F,  1.0F,
		 1.0F,  1.0F,
		 1.0F, -1.0F,
		-1.0F, -1.0F,
	};

	std::array const tetramino_I_data = {
		-1.5F,  0.5F,
		 1.5F,  0.5F,
		 1.5F, -0.5F,
		-1.5F, -0.5F,
	};

	std::array const tetramino_L_data = {
		-0.5F,  0.5F,
		 1.5F,  0.5F,
		 1.5F, -0.5F,
		-0.5F, -0.5F,

		-0.5F, -0.5F,
		 0.5F, -0.5F,
		 0.5F, -2.5F,
		-0.5F, -2.5F,
	};

	std::array const tetramino_Z_data = {
		 0.0F, -1.5F,
		 1.0F, -1.5F,
		 1.0F,  0.5F,
		 0.0F,  0.5F,

		-1.0F, -0.5F,
		 0.0F, -0.5F,
		 0.0F,  1.5F,
		-1.0F,  1.5F,
	};

	std::array const tetramino_T_data = {
		-1.5F,  0.5F,
		 1.5F,  0.5F,
		 1.5F, -0.5F,
		-1.5F, -0.5F,

		-0.5F, -1.5F,
		 0.5F, -1.5F,
		 0.5F, -0.5F,
		-0.5F, -0.5F,
	};

	std::array const quad_idx_data = {
		0, 1, 2,
		0, 2, 3,
	};

	std::array const two_quad_idx_data = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
	};

	VertexBufferLayout layout;
	layout.Push<float>(2);

	VertexArray const tetramino_square_vao(tetramino_square_data, quad_idx_data, layout);
	VertexArray const tetramino_I_vao(tetramino_I_data, quad_idx_data, layout);
	VertexArray const tetramino_L_vao(tetramino_L_data, two_quad_idx_data, layout);
	VertexArray const tetramino_Z_vao(tetramino_Z_data, two_quad_idx_data, layout);
	VertexArray const tetramino_T_vao(tetramino_T_data, two_quad_idx_data, layout);

	tetramino_square_vao.Bind();

	Shader test_shader("res/shaders/color.vert", "res/shaders/color.frag");
	test_shader.Bind();
	test_shader.SetFloat4("u_Color", glm::vec4(0.2F, 0.8F, 0.4F, 1.0F));

	glm::vec3 scale(0.1F);
	glm::vec3 pos(0.0f);

	auto UpdateTime = [last_time = 0.0]() mutable {
		float const delta_time = static_cast<float>(glfwGetTime() - last_time);
		last_time = glfwGetTime();
		return delta_time;
	};

	while (!glfwWindowShouldClose(_window))
	{
		float const dt = UpdateTime();

		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		if (GetKey(GLFW_KEY_RIGHT).held)
		{
			pos.x += 10.0F * dt;
		}
		if (GetKey(GLFW_KEY_LEFT).held)
		{
			pos.x -= 10.0F * dt;
		}

		if (GetKey(GLFW_KEY_UP).held)
		{
			scale.x += 1.0F * dt;
			scale.y += 1.0F * dt;
			scale.z += 1.0F * dt;
		}

		if (GetKey(GLFW_KEY_DOWN).held)
		{
			scale.x -= 1.0F * dt;
			scale.y -= 1.0F * dt;
			scale.z -= 1.0F * dt;
		}

		glm::mat4 model = glm::mat4(1.0F);
		model = glm::scale(model, glm::vec3(0.25F, 0.25F, 0.25F));
		model = glm::scale(model, scale);
		model = glm::translate(model, pos);

		tetramino_square_vao.Bind();
		model = glm::translate(model, glm::vec3(-2.5F, 2.5F, 0.0F));
		test_shader.SetMat4("u_Model", model);
		test_shader.SetFloat4("u_Color", glm::vec4(0.99F, 0.9F, 0.3F, 1.0F));
		glDrawElements(GL_TRIANGLES, tetramino_square_vao.Count(), GL_UNSIGNED_INT, 0);

		tetramino_I_vao.Bind();
		model = glm::translate(model, glm::vec3(4.0F, 0.0F, 0.0F));
		test_shader.SetMat4("u_Model", model);
		test_shader.SetFloat4("u_Color", glm::vec4(0.4F, 0.97F, 0.95F, 1.0F));
		glDrawElements(GL_TRIANGLES, tetramino_I_vao.Count(), GL_UNSIGNED_INT, 0);

		tetramino_L_vao.Bind();
		model = glm::translate(model, glm::vec3(-4.5F, -2.0F, 0.0F));
		test_shader.SetMat4("u_Model", model);
		test_shader.SetFloat4("u_Color", glm::vec4(1.0F, 0.6F, 0.2F, 1.0F));
		glDrawElements(GL_TRIANGLES, tetramino_L_vao.Count(), GL_UNSIGNED_INT, 0);

		tetramino_Z_vao.Bind();
		model = glm::translate(model, glm::vec3(5.5F, -1.0F, 0.0F));
		test_shader.SetMat4("u_Model", model);
		test_shader.SetFloat4("u_Color", glm::vec4(0.1F, 1.0F, 0.4F, 1.0F));
		glDrawElements(GL_TRIANGLES, tetramino_Z_vao.Count(), GL_UNSIGNED_INT, 0);

		tetramino_T_vao.Bind();
		model = glm::translate(model, glm::vec3(-3.0F, -1.0F, 0.0F));
		test_shader.SetMat4("u_Model", model);
		test_shader.SetFloat4("u_Color", glm::vec4(1.0F, 0.45F, 0.9F, 1.0F));
		glDrawElements(GL_TRIANGLES, tetramino_T_vao.Count(), GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(_window);
	}
}
