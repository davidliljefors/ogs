#pragma once

#include <GLFW/glfw3.h>
#include <array>

namespace ogs {

class Input {

public:
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

	std::array<KeyState, GLFW_KEY_LAST> _keys;
};
}