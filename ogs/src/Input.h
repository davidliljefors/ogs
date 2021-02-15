#pragma once

#include <GLFW/glfw3.h>
#include <array>

namespace ogs {

class Input {

public:

	inline void Update()
	{
		_mouse_delta = glm::vec2(0.0F);
	}

	inline void MouseMove(double x, double y)
	{
		auto const mouse_pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
		if (_first_mouse)
		{
			_last_mouse_pos = mouse_pos;
			_first_mouse = false;
		}

		_mouse_delta = mouse_pos - _last_mouse_pos;
		_last_mouse_pos = mouse_pos;
	}

	inline auto GetMouseDelta() const
	{
		return _mouse_delta;
	}

	inline auto GetMousePos() const
	{
		return _last_mouse_pos;
	}

	void KeyEvent(int key, int action)
	{
		if (action == GLFW_PRESS)
		{
			_keys[key].held = true;
			return;
		}

		if (action == GLFW_RELEASE)
		{
			_keys[key].held = false;
			return;
		}
	};

	auto GetKey(int key)
	{
		return _keys[key];
	}

private:
	struct KeyState
	{
		bool held = false;
	};

	std::array<KeyState, GLFW_KEY_LAST> _keys {};
	glm::vec2 _last_mouse_pos = glm::vec2(0.0F);
	glm::vec2 _mouse_delta = glm::vec2(0.0F);

	bool _first_mouse = true;
};
}