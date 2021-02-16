#pragma once

#include <GLFW/glfw3.h>
#include <array>

namespace ogs {

class Input {

public:

	inline void Update()
	{
		_mouse_delta = glm::vec2(0.0F);

		for (auto& key_sate : _keys)
		{
			key_sate.pressed = false;
			key_sate.released = false;
		}
	}

	inline void MouseMove(float x, float y)
	{
		auto const mouse_pos = glm::vec2(x, y);
		if (_first_mouse)
		{
			_last_mouse_pos = mouse_pos;
			_first_mouse = false;
		}

		_mouse_delta += (mouse_pos - _last_mouse_pos);
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
			_keys[key].pressed = true;
			_keys[key].held = true;
			return;
		}

		if (action == GLFW_RELEASE)
		{
			_keys[key].held = false;
			_keys[key].released = true;
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
		// True the frame key got pressed
		bool pressed = false;
		// True while key is being held
		bool held = false;
		// True the frame key got released
		bool released = false;
	};

	std::array<KeyState, GLFW_KEY_LAST> _keys {};
	glm::vec2 _last_mouse_pos = glm::vec2(0.0F);
	glm::vec2 _mouse_delta = glm::vec2(0.0F);

	bool _first_mouse = true;
};
}