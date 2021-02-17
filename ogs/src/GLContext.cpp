#include "GLContext.h"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

//ImGui 
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

bool newframe = true;

ogs::GLContext::GLContext(WindowProps window_props)
	:_window_props(window_props), _camera(_window_props.GetAspect(), 90.0F)
{
	_data.input = &_input;
	_data.window_props = &_window_props;
	_data.camera = &_camera;

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	_window = glfwCreateWindow(window_props.GetWidth(), window_props.GetHeight(), "ogs", nullptr, nullptr);

	if (!_window)
	{
		glfwTerminate();
		LogError("GLFW : Create Window Error");
		std::exit(-1);
	}

	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, &_data);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LogError("GLAD : Failed to initialize GLAD");
		std::exit(-1);
	}

	{ // Setup callback funcs

	auto ResizeCallback = [](GLFWwindow* window, int xsize, int ysize)
	{
		auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
		assert(user_data && "Window User Pointer should not be null!");
		*user_data->window_props = { xsize, ysize };
		user_data->camera->SetAspectRatio(user_data->window_props->GetAspect());
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
		}
	};

	glfwSetMouseButtonCallback(_window, MouseButtonCallback);

	auto ScrollCallback = [](GLFWwindow* window, double , double yoffset)
	{
		auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
		assert(user_data && "Window User Pointer should not be null!");
		user_data->camera->Zoom(static_cast<float>(yoffset));
	};

	glfwSetScrollCallback(_window, ScrollCallback);

	auto MouseMoveCallback = [](GLFWwindow* window, double xpos, double ypos)
	{
		auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
		user_data->input->MouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
	};

	glfwSetCursorPosCallback(_window, MouseMoveCallback);

	} // Setup callback funcs



	//glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(0);

	if (glfwRawMouseMotionSupported())
	{
		LogHint("Raw input enabled");
		glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	// Initialize ImGui
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();


	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
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
	
	_camera.SetPosition(glm::vec3(0.0F, 0.0F, 2.0F));

	glm::vec3 offset{};
	glm::vec4 tint{};

	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		auto const dt = UpdateTime();
		auto const time = static_cast<float>(glfwGetTime());
		test_shader.SetFloat("u_Time", time);

		auto const mouse_move_delta = _input.GetMouseDelta();
		if (mouse_move_delta.length() > 0.01F)
		{
			_camera.Rotate(mouse_move_delta * _mouse_sensitivity);
		}
		
		auto camera_movement = glm::vec3(0.0F);

		if (GetKey(GLFW_KEY_W).held)
		{
			camera_movement.y = 1.0F;
		}

		if (GetKey(GLFW_KEY_S).held)
		{
			camera_movement.y = -1.0F;
		}
		if (GetKey(GLFW_KEY_A).held)
		{
			camera_movement.x = 1.0F;
		}

		if (GetKey(GLFW_KEY_D).held)
		{
			camera_movement.x = -1.0F;
		}

		if (camera_movement.length() > 0.01F)
		{
			_camera.Move(camera_movement * 3.0F * dt);
		}

		if (GetKey(GLFW_KEY_SPACE).pressed)
		{
			LogHint("Jump Press!");
		}

		if (GetKey(GLFW_KEY_SPACE).released)
		{
			LogHint("Jump Release!");
		}

		// User Loop

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		test_shader.SetMat4("u_VP", _camera.GetVP());
		test_shader.SetFloat4("u_Color", tint);
		OnUpdate(dt);
		

		ImGui::Begin("Settings");
		ImGui::DragFloat3("Offset position", glm::value_ptr(offset));
		ImGui::ColorEdit4("Tint", glm::value_ptr(tint));
		ImGui::End();

		for (int y = -10; y < 10; ++y)
		{
			for (int x = -10; x < 10; ++x)
			{
				auto const pos = offset + glm::vec3(static_cast<float>(x) * 1.1F, static_cast<float>(y) * 1.1F, 0.0F);


				auto model = glm::translate(glm::mat4(1.0F), pos);
				model = model * model;
				model = glm::rotate(model, x / 5.0F + time, glm::vec3{ 0.0F, 1.0F, 0.0F });
				test_shader.SetMat4("u_Model", model);
				quad_vao.Bind();
				glDrawElements(GL_TRIANGLES, quad_vao.Count(), GL_UNSIGNED_INT, nullptr);
			}
		}

		// User Loop end

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(GetViewport().x, GetViewport().y);

		// Rendering	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		_input.Update();
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}
}
