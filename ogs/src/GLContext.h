#pragma once
#include <glm/glm.hpp>

#include "AssetLibrary.h"
#include "Camera.h"
#include "Input.h"
#include "Shader.h"

namespace ogs {

class WindowProps
{
public:
	constexpr WindowProps(int width, int height)
		: m_height(height), m_width(width), m_aspect(width / static_cast<float>(height))
	{};

	constexpr auto GetWidth() const { return m_width; };
	constexpr auto GetHeight() const { return m_height; };
	constexpr auto GetAspect() const { return m_aspect; };

private:
	int m_width;
	int m_height;
	float m_aspect;
};

constexpr static WindowProps DefaultWindowProp = { 600, 400 };

class GLContext
{
public:
	struct WindowUserData
	{
		Input* input = nullptr;
		Camera* camera = nullptr;
		WindowProps* window_props = nullptr;
	};

public:
	GLContext() = default;
	void Construct(WindowProps window_props);
	virtual ~GLContext();

	void Run();

	inline auto GetViewport()
	{
		return glm::vec2(m_windowProps.GetWidth(), m_windowProps.GetHeight());
	}

	float GetTime();

	inline auto GetAspect()
	{
		return m_windowProps.GetAspect();
	}

protected:
	virtual void OnConstruct() {};
	virtual void OnUpdate(float) {};

	inline auto GetKey(int glfw_key)
	{
		return m_input.GetKey(glfw_key);
	}

protected:
	GLFWwindow* m_pWindow = nullptr;
	WindowUserData m_windowUserData{};
	Input m_input{};
	WindowProps m_windowProps = DefaultWindowProp;
	Camera m_camera = { DefaultWindowProp.GetAspect(), 90.0F };
	float m_mouseSensitivity = 0.07F;

	AssetLibrary assets;

private:
	std::unique_ptr<Shader> m_pShader;
	bool m_bInitialized = false;
};

}
