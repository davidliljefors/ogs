#pragma once
#include <glm/glm.hpp>

#include "Input.h"
#include "Camera.h"

namespace ogs {
	
class WindowProps
{
public:
	constexpr WindowProps(int width, int height)
		:_height(height), _width(width), _aspect(width / static_cast<float>(height))
	{};

	constexpr auto GetWidth()  const { return _width; };
	constexpr auto GetHeight() const { return _height; };
	constexpr auto GetAspect() const { return _aspect; };

private:
	int _width;
	int _height;
	float _aspect;
};

constexpr static WindowProps DefaultWindowProp = { 600, 400 };

class GLContext {
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
		return glm::vec2(_window_props.GetWidth(), _window_props.GetHeight());
	}

	float GetTime();

	inline auto GetAspect()
	{
		return _window_props.GetAspect();
	}

protected:
	virtual void OnConstruct() {};
	virtual void OnUpdate(float) {};

protected:
	inline auto GetKey(int glfw_key)
	{
		return _input.GetKey(glfw_key);
	}

protected:
	GLFWwindow* _window = nullptr;
	WindowUserData _data{};
	Input _input{};
	WindowProps _window_props = DefaultWindowProp;
	Camera _camera = { DefaultWindowProp.GetAspect(), 90.0F };
	float _mouse_sensitivity = 0.07F;

private:
	bool initialized = false;
};
}
