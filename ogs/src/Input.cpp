#include "ogspch.h"

#include "Input.h"

void ogs::Input::Update()
{
	m_mouseDelta = glm::vec2(0.0F);

	for (auto& key_sate : m_keys)
	{
		key_sate.bPressed = false;
		key_sate.bReleased = false;
	}

	for (auto& mouse_state : m_mouseButtons)
	{
		mouse_state.bPressed = false;
		mouse_state.bReleased = false;
	}
}

void ogs::Input::MouseMove(float x, float y)
{
	auto const mouse_pos = glm::vec2(x, y);
	if (m_bFirstMouse)
	{
		m_lastMousePos = mouse_pos;
		m_bFirstMouse = false;
	}

	m_mouseDelta += (mouse_pos - m_lastMousePos);
	m_lastMousePos = mouse_pos;
}

void ogs::Input::KeyEvent(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		m_keys[key].bPressed = true;
		m_keys[key].bHeld = true;
		return;
	}

	if (action == GLFW_RELEASE)
	{
		m_keys[key].bHeld = false;
		m_keys[key].bReleased = true;
		return;
	}
}

void ogs::Input::MouseEvent(int button, int action)
{
	if (action == GLFW_PRESS)
	{
		m_mouseButtons[button].bPressed = true;
		m_mouseButtons[button].bHeld = true;
		return;
	}

	if (action == GLFW_RELEASE)
	{
		m_mouseButtons[button].bHeld = false;
		m_mouseButtons[button].bReleased = true;
		return;
	}
}
