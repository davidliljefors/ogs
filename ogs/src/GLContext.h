#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <utility>
#include <array>

#include "Input.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"

namespace ogs {

class GLContext {
public:
	class WindowProps
	{
	public:
		WindowProps(int width, int height)
			:_height(height), _width(width), _aspect(width / static_cast<float>(height))
		{};

		inline auto GetWidth()  const { return _width; };
		inline auto GetHeight() const { return _height; };
		inline auto GetAspect() const { return _aspect; };

	private:
		int _width;
		int _height;
		float _aspect;
	};

	struct WindowUserData
	{
		Input* input;
		WindowProps* window_props;
	};

public:
	GLContext(WindowProps window_props);
	~GLContext();

	void Run();

	inline auto GetViewport()
	{
		return glm::vec2(_window_props.GetWidth(), _window_props.GetHeight());
	}

	inline auto GetAspect()
	{
		return _window_props.GetAspect();
	}

private:
	inline auto GetKey(int glfw_key)
	{
		return _input.GetKey(glfw_key);
	}

private:
	GLFWwindow* _window = nullptr;
	WindowUserData _data;
	Input _input;
	WindowProps _window_props;
	Camera _camera;
};
}