#include "ogspch.h"
#include "GLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Renderer.h"

//ImGui 
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


void ogs::GLContext::Construct(WindowProps window_props)
{
	m_windowProps = window_props;
	m_camera = { window_props.GetAspect(), 90.0F };

	m_windowUserData.input = &m_input;
	m_windowUserData.window_props = &m_windowProps;
	m_windowUserData.camera = &m_camera;

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_pWindow = glfwCreateWindow(window_props.GetWidth(), window_props.GetHeight(), "ogs", nullptr, nullptr);

	if (!m_pWindow)
	{
		glfwTerminate();
		LogError("GLFW : Create Window Error");
		std::exit(-1);
	}

	glfwMakeContextCurrent(m_pWindow);
	glfwSetWindowUserPointer(m_pWindow, &m_windowUserData);

	glfwSwapInterval(1);

	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (glfwRawMouseMotionSupported())
	{
		LogHint("Raw input enabled");
		glfwSetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LogError("GLAD : Failed to initialize GLAD");
		std::exit(-1);
	}

	glEnable(GL_DEPTH_TEST);
	m_bInitialized = true;

	// Setup callback funcs
	{

		auto ResizeCallback = [](GLFWwindow* window, int xsize, int ysize) {
			auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
			assert(user_data && "Window User Pointer should not be null!");
			*user_data->window_props = { xsize, ysize };
			user_data->camera->SetAspectRatio(user_data->window_props->GetAspect());
			glViewport(0, 0, xsize, ysize);
		};

		glfwSetWindowSizeCallback(m_pWindow, ResizeCallback);

		auto KeypressCallback = [](GLFWwindow* window, int key, int, int action, int mods) {
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureKeyboard)
				return;
			auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
			assert(user_data && "Window User Pointer should not be null!");

			user_data->input->KeyEvent(key, action);

			if (action != GLFW_PRESS)
			{
				return;
			}

			if (key == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(window, true);
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

		glfwSetKeyCallback(m_pWindow, KeypressCallback);

		auto MouseButtonCallback = [](GLFWwindow* window, int button, int action, int) {
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse)
				return;
			auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
			assert(user_data && "Window User Pointer should not be null!");
			user_data->input->MouseEvent(button, action);

			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		};

		glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);

		auto ScrollCallback = [](GLFWwindow* window, double, double yoffset) {
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse)
				return;
			auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
			assert(user_data && "Window User Pointer should not be null!");

			user_data->camera->Zoom(static_cast<float>(yoffset));
		};

		glfwSetScrollCallback(m_pWindow, ScrollCallback);

		auto MouseMoveCallback = [](GLFWwindow* window, double xpos, double ypos) {
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse)
				return;
			auto user_data = static_cast<WindowUserData*> (glfwGetWindowUserPointer(window));
			user_data->input->MouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
		};

		glfwSetCursorPosCallback(m_pWindow, MouseMoveCallback);

	} // Setup callback funcs


	// Initialize ImGui
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	auto screen_dpi_scale = [window = m_pWindow]() {
		float xscale{ 1.0f };
		float yscale{ 1.0f };
		int monitor_count;
		const auto monitors = glfwGetMonitors(&monitor_count);
		if (monitor_count > 0)
		{
			glfwGetMonitorContentScale(monitors[0], &xscale, &yscale);
		}
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

	ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	m_pShader = std::make_unique<Shader>("res/shaders/default.vert", "res/shaders/default.frag");
	Renderer::UseShader(*m_pShader);

	OnConstruct();
}

ogs::GLContext::~GLContext()
{
	if (m_bInitialized)
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

	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto const dt = UpdateTime();

		// Camera Movement
		if (glfwGetInputMode(m_pWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			auto const mouse_move_delta = m_input.GetMouseDelta();
			if (length(mouse_move_delta) > 0.01F)
			{
				m_camera.Rotate(mouse_move_delta * m_mouseSensitivity);
			}
			auto camera_movement = glm::vec3(0.0F);
			if (GetKey(GLFW_KEY_W).bHeld)
			{
				camera_movement.z = 1.0F;
			}

			if (GetKey(GLFW_KEY_S).bHeld)
			{
				camera_movement.z = -1.0F;
			}
			if (GetKey(GLFW_KEY_A).bHeld)
			{
				camera_movement.x = 1.0F;
			}

			if (GetKey(GLFW_KEY_D).bHeld)
			{
				camera_movement.x = -1.0F;
			}

			if (GetKey(GLFW_KEY_Q).bHeld)
			{
				camera_movement.y = 1.0F;
			}

			if (GetKey(GLFW_KEY_Z).bHeld)
			{
				camera_movement.y = -1.0F;
			}

			if (camera_movement.length() > 0.01F)
			{
				if (GetKey(GLFW_KEY_LEFT_SHIFT).bHeld)
					m_camera.Move(camera_movement * 25.0F * dt);
				else
					m_camera.Move(camera_movement * 5.0F * dt);

			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// User Update Loop
		Renderer::BeginScene(m_camera);
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

		m_input.Update();
		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
}

float ogs::GLContext::GetTime()
{
	return static_cast<float>(glfwGetTime());
}
