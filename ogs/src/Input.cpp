#include "ogspch.h"
#include "Input.h"

void ogs::Input::Update()
{
	_mouse_delta = glm::vec2( 0.0F );

	for ( auto& key_sate : _keys )
	{
		key_sate.pressed = false;
		key_sate.released = false;
	}

	for ( auto& mouse_state : _mouse_buttons )
	{
		mouse_state.pressed = false;
		mouse_state.released = false;
	}
}

void ogs::Input::MouseMove( float x, float y )
{
	auto const mouse_pos = glm::vec2( x, y );
	if ( _first_mouse )
	{
		_last_mouse_pos = mouse_pos;
		_first_mouse = false;
	}

	_mouse_delta += (mouse_pos - _last_mouse_pos);
	_last_mouse_pos = mouse_pos;
}

void ogs::Input::KeyEvent( int key, int action )
{
	if ( action == GLFW_PRESS )
	{
		_keys[key].pressed = true;
		_keys[key].held = true;
		return;
	}

	if ( action == GLFW_RELEASE )
	{
		_keys[key].held = false;
		_keys[key].released = true;
		return;
	}
}

void ogs::Input::MouseEvent( int button, int action )
{
	if ( action == GLFW_PRESS )
	{
		_mouse_buttons[button].pressed = true;
		_mouse_buttons[button].held = true;
		return;
	}

	if ( action == GLFW_RELEASE )
	{
		_mouse_buttons[button].held = false;
		_mouse_buttons[button].released = true;
		return;
	}
}
