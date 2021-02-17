#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ogs {

class Camera {
public:
	Camera(float aspect_ratio)
		:_position(glm::vec3(0.0F))
		,_forward(0.0F, 0.0F, -1.0F)
	{
		SetAspectRatio(aspect_ratio);
		UpdateMatrix();
	}

	inline void Rotate(glm::vec2 mouse_delta)
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

	inline void Move(glm::vec3 local_move)
	{
		auto const up = glm::vec3(0.0F, 1.0F, 0.0F);
		auto const rot = glm::mat3(glm::cross(up, _forward), _forward, up);
		auto const rotated_move = rot * local_move;
		_position += rotated_move;
		_dirty = true;
	}

	inline void SetAspectRatio(float aspect_ratio)
	{
		_aspect = aspect_ratio;
		_projection = glm::perspective(glm::radians(90.0F), _aspect, 0.1F, 100.F);
	}

	inline void SetPosition(glm::vec3 const& position)
	{
		_position = position;
		_dirty = true;
	}

	inline auto const& GetVP()
	{
		if(_dirty)
		{
			UpdateMatrix();
			_dirty = false;
		}
		return _view_projection;
	}

private:
	inline void UpdateMatrix()
	{
		auto const view = glm::lookAt(_position, _position+_forward, glm::vec3(0.0F, 1.0F, 0.0F));
		_view_projection = _projection * view;
	}

private:
	glm::mat4 _projection;
	glm::mat4 _view_projection;
	glm::vec3 _position;
	glm::vec3 _forward;

	float _aspect;
	float _yaw = -90.0F;
	float _pitch = 0.0f;
	bool _dirty = true;
};

}