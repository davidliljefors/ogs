#include "GLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <array>

#include "Renderer.h"

//ImGui 
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


void ogs::GLContext::Construct(WindowProps window_props)
{
	_window_props = window_props;
	_camera = { window_props.GetAspect(), 90.0F };

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

	glfwSwapInterval(1);

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glfwRawMouseMotionSupported())
	{
		LogHint("Raw input enabled");
		glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LogError("GLAD : Failed to initialize GLAD");
		std::exit(-1);
	}

	glEnable(GL_DEPTH_TEST);
	initialized = true;

	
	// Setup callback funcs
	{ 

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
				if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
				{
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				else
				{
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
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

		auto ScrollCallback = [](GLFWwindow* window, double, double yoffset)
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


	// Initialize ImGui
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	
	ImGui::StyleColorsDark();
	
	auto screen_dpi_scale = [window = _window]()
	{
		float xscale, yscale;
		int monitor_count;
		auto monitors = glfwGetMonitors(&monitor_count);
		glfwGetMonitorContentScale(monitors[0], &xscale, &yscale);
		return xscale;
	}();


	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 4.0f * screen_dpi_scale;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		style.ScaleAllSizes(screen_dpi_scale);
		io.Fonts->AddFontFromFileTTF("res/fonts/Verdana.ttf", 18.0f * screen_dpi_scale, nullptr, nullptr);
	}

	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	OnConstruct();
}

ogs::GLContext::~GLContext()
{
	if (initialized)
	{
		glfwTerminate();
	}
}

void ogs::GLContext::Run()
{
	auto UpdateTime = [last_time = 0.0]() mutable {
		auto const delta_time = static_cast<float>(glfwGetTime() - last_time);
		last_time = glfwGetTime();
		return delta_time;
	};
	
	_camera.SetPosition(glm::vec3(0.0F, 0.0F, 2.0F));

	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto const dt = UpdateTime();
		
		// Camera Movement
		if (glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
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
		}
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// User Update Loop
		OnUpdate(dt);
		

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

float ogs::GLContext::GetTime()
{
	return static_cast<float>(glfwGetTime());
}
