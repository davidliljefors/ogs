#include "ogspch.h"
#include "Camera.h"

ogs::Camera::Camera(float aspect_ratio, float fov_y) :
	_position(glm::vec3(0.0F)),
	_forward(0.0F, 0.0F, -1.0F),
	_fov(fov_y),
	_current_fov(_fov)
{
	SetAspectRatio(aspect_ratio);
	UpdateMatrix();
}

void ogs::Camera::Rotate(glm::vec2 mouse_delta)
{
	glm::vec3 forward(0.0F);
	_yaw += mouse_delta.x;
	_pitch -= mouse_delta.y;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	forward.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	forward.y = sin(glm::radians(_pitch));
	forward.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_forward = glm::normalize(forward);
	_dirty = true;
}

void ogs::Camera::Zoom(float delta)
{
	_current_fov -= delta;
	_current_fov = std::min(_current_fov, _fov);
	_current_fov = std::max(_current_fov, MIN_FOV);
	_dirty = true;
}

void ogs::Camera::Move(glm::vec3 local_move)
{
	auto const right_vector = glm::normalize(glm::cross(glm::vec3(0.0F, 1.0F, 0.0F), _forward));

	_position += _forward * local_move.z;
	_position += right_vector * local_move.x;
	_position.y += local_move.y;
	_dirty = true;
}

void ogs::Camera::SetAspectRatio(float aspect_ratio)
{
	_aspect = aspect_ratio;
	_dirty = true;
}

void ogs::Camera::SetPosition(glm::vec3 const& position)
{
	_position = position;
	_dirty = true;
}

glm::mat4 const& ogs::Camera::GetVP() const
{
	if (_dirty)
	{
		UpdateMatrix();
		_dirty = false;
	}
	return _view_projection;
}

void ogs::Camera::UpdateMatrix() const
{
	auto const projection = glm::perspective(glm::radians(_current_fov), _aspect, 0.1F, 100.F);
	auto const view = glm::lookAt(_position, _position + _forward, glm::vec3(0.0F, 1.0F, 0.0F));
	_view_projection = projection * view;
}
