#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ogs {

class Camera {
public:
	Camera( float aspect_ratio, float fov_y );

	void Rotate( glm::vec2 mouse_delta );

	void Zoom( float delta );

	void Move( glm::vec3 local_move );

	void SetAspectRatio( float aspect_ratio );

	void SetPosition( glm::vec3 const& position );

	glm::mat4 const& GetVP() const;

	auto const& GetPosition() const { return _position; }

private:
	void UpdateMatrix() const;

private:
	mutable glm::mat4 _view_projection;
	glm::vec3 _position;
	glm::vec3 _forward;

	float _fov;
	float _current_fov;
	float _aspect;
	float _yaw = -90.0F;
	float _pitch = 0.0f;
	mutable bool _dirty = true;

	static constexpr float MIN_FOV = 10.0F;
};

}