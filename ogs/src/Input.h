#pragma once

#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>

namespace ogs {

class Input {

public:

	void Update();

	void MouseMove( float x, float y );

	inline auto GetMouseDelta() const
	{
		return _mouse_delta;
	}

	inline auto GetMousePos() const
	{
		return _last_mouse_pos;
	}

	void KeyEvent( int key, int action );

	void MouseEvent( int button, int action );

	auto GetKey( int key )
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

	std::array<KeyState, GLFW_KEY_LAST> _keys{};
	std::array<KeyState, GLFW_MOUSE_BUTTON_LAST> _mouse_buttons{};
	glm::vec2 _last_mouse_pos = glm::vec2( 0.0F );
	glm::vec2 _mouse_delta = glm::vec2( 0.0F );

	bool _first_mouse = true;
};
}