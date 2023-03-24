#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ogs {

class Camera
{
public:
	Camera(float aspect_ratio, float fov_y);

	void Rotate(glm::vec2 mouse_delta);

	void Zoom(float delta);

	void Move(glm::vec3 local_move);

	void SetAspectRatio(float aspect_ratio);

	void SetPosition(glm::vec3 const& position);

	glm::mat4 const& GetVP() const;

	glm::vec3 const& GetPosition() const { return m_pos; }

private:
	void RecalculateViewProjection() const;

private:
	mutable glm::mat4 m_vp;
	mutable int m_revision = 0;
	mutable int m_lastRevision = -1;

	glm::vec3 m_pos = { -12.0f, 8.0f, -12.0f };
	glm::vec3 m_forward;

	float m_fov = 90.0f;
	float m_currentFov = 90.0f;
	float m_aspect = 16.0f/9.0f;
	float m_yaw = -90.0F;
	float m_pitch = 0.0f;


	static constexpr float MIN_FOV = 10.0F;
};

}