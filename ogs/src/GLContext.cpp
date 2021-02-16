#include <Windows.h>
#include "GLContext.h"

#include <chrono>
#include <thread>


bool newframe = true;

ogs::GLContext::GLContext(WindowProps window_props)
	:_window_props(window_props), _camera(_window_props.GetAspect())
{
	_data.input = &_input;
	_data.window_props = &_window_props;

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	_window = glfwCreateWindow(window_props.GetWidth(), window_props.GetHeight(), "ogs", nullptr, nullptr);

	if (!_window)
	{
		glfwTerminate();
		std::cerr << "GLFW : Create Window Error" << std::endl;
		std::exit(-1);
	}

	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, &_data);
	

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "GLAD : Failed to initialize GLAD" << std::endl;
		std::exit(-1);
	}

	auto ResizeCallback = [](GLFWwindow* window, int xsize, int ysize)
	{
		auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
		assert(user_data && "Window User Pointer should not be null!");
		*user_data->window_props = { xsize, ysize };
		glViewport(0, 0, xsize, ysize);
	};

	glfwSetWindowSizeCallback(_window, ResizeCallback);

	auto KeypressCallback = [](GLFWwindow* window, int key, int, int action, int mods)
	{
		auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
		assert(user_data && "Window User Pointer should not be null!");

		user_data->input->KeyEvent(key, action);

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

	auto MouseMoveCallback = [](GLFWwindow* window, double xpos, double ypos)
	{
		auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
		user_data->input->MouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
	};

	glfwSetCursorPosCallback(_window, MouseMoveCallback);

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);

	if (glfwRawMouseMotionSupported())
	{
		std::cout << "Raw input enabled\n";
		glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	glEnable(GL_MULTISAMPLE);
}

ogs::GLContext::~GLContext()
{
	glfwTerminate();
}

void ogs::GLContext::Run()
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

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	VertexArray const quad_vao(quad_vertex_data, quad_idx_data, layout);
	quad_vao.Bind();

	Texture const texture_ora("res/textures/very_nice_texture.png");
	Texture const texture_blu("res/textures/cool_texture.png");
	texture_ora.Bind(0);
	texture_blu.Bind(1);

	Shader test_shader("res/shaders/color.vert", "res/shaders/color.frag");
	test_shader.Bind();
	test_shader.SetInt("u_Tex0", 0);
	test_shader.SetInt("u_Tex1", 1);

	auto UpdateTime = [last_time = 0.0]() mutable {
		auto const delta_time = static_cast<float>(glfwGetTime() - last_time);
		last_time = glfwGetTime();
		return delta_time;
	};

	glm::vec3 scale(5.0F);
	glm::vec3 pos(0.0F);

	_camera.SetPosition(glm::vec3(0.0F, 0.0F, 2.0F));

	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		auto const dt = UpdateTime();
		auto const time = static_cast<float>(glfwGetTime());
		test_shader.SetFloat("u_Time", time);

		auto const camera_rot = _input.GetMouseDelta() * _mouse_sensitivity;
		if (camera_rot.length() > 0.01F)
		{
			_camera.Rotate(camera_rot);
		}
		
		auto camera_movement = glm::vec3(0.0F);
		if (GetKey(GLFW_KEY_W).held)
		{
			camera_movement.y = 3.0F * dt;
		}

		if (GetKey(GLFW_KEY_S).held)
		{
			camera_movement.y = -3.0F * dt;
		}
		if (GetKey(GLFW_KEY_A).held)
		{
			camera_movement.x = 3.0F * dt;
		}

		if (GetKey(GLFW_KEY_D).held)
		{
			camera_movement.x = -3.0F * dt;
		}

		if (camera_movement.length() > 0.01F)
		{
			_camera.Move(camera_movement);
		}

		if (GetKey(GLFW_KEY_SPACE).pressed)
		{
			std::cout << "Jump!\n";
		}

		if (GetKey(GLFW_KEY_SPACE).released)
		{
			std::cout << "Jump release!\n";
		}

		auto model = glm::translate(glm::mat4(1.0F), pos);
		model = glm::scale(model, scale);

		auto const MVP = _camera.GetVP() * model;
		test_shader.SetMat4("u_MVP", MVP);
		test_shader.SetFloat4("u_Color", { 0.99F, 0.9F, 0.3F, 1.0F });

		quad_vao.Bind();
		glDrawElements(GL_TRIANGLES, quad_vao.Count(), GL_UNSIGNED_INT, nullptr);

		_input.Update();
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}
}
