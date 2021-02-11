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


class GLContext
{
public:

	GLContext();
	~GLContext();
	void Run();

private:
	auto GetKey(int key)
	{
		return _input.GetKey(key);
	}

private:
	GLFWwindow* _window = nullptr;
	Input _input;
};