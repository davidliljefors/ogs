#pragma once

#include <GLFW/glfw3.h>
#include <array>
#include <glm/vec2.hpp>

namespace ogs {

class Input
{

public:

	void Update();

	void MouseMove(float x, float y);

	inline auto GetMouseDelta() const
	{
		return m_mouseDelta;
	}

	inline auto GetMousePos() const
	{
		return m_lastMousePos;
	}

	void KeyEvent(int key, int action);

	void MouseEvent(int button, int action);

	auto GetKey(int key)
	{
		return m_keys[key];
	}

private:
	struct KeyState
	{
		// True the frame key got pressed
		bool bPressed = false;
		// True while key is being held
		bool bHeld = false;
		// True the frame key got released
		bool bReleased = false;
	};

	std::array<KeyState, GLFW_KEY_LAST> m_keys{};
	std::array<KeyState, GLFW_MOUSE_BUTTON_LAST> m_mouseButtons{};
	glm::vec2 m_lastMousePos = glm::vec2(0.0F);
	glm::vec2 m_mouseDelta = glm::vec2(0.0F);

	bool m_bFirstMouse = true;
};
}