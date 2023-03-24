#include "ogspch.h"
#include "Camera.h"

ogs::Camera::Camera(float aspect_ratio, float fov_y) :
	m_fov(fov_y),
	m_currentFov(m_fov)
{
	m_pitch = -30;
	m_yaw = 45;
	glm::vec3 forward(0.0F);
	forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	forward.y = sin(glm::radians(m_pitch));
	forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_forward = glm::normalize(forward);

	SetAspectRatio(aspect_ratio);
	RecalculateViewProjection();
}

void ogs::Camera::Rotate(glm::vec2 mouse_delta)
{
	glm::vec3 forward(0.0F);
	m_yaw += mouse_delta.x;
	m_pitch -= mouse_delta.y;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	forward.y = sin(glm::radians(m_pitch));
	forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_forward = glm::normalize(forward);
	++m_revision;
}

void ogs::Camera::Zoom(float delta)
{
	m_currentFov -= delta;
	m_currentFov = std::min(m_currentFov, m_fov);
	m_currentFov = std::max(m_currentFov, MIN_FOV);
	++m_revision;
}

void ogs::Camera::Move(glm::vec3 local_move)
{
	auto const right_vector = glm::normalize(glm::cross(glm::vec3(0.0F, 1.0F, 0.0F), m_forward));

	m_pos += m_forward * local_move.z;
	m_pos += right_vector * local_move.x;
	m_pos.y += local_move.y;
	++m_revision;
}

void ogs::Camera::SetAspectRatio(float aspect_ratio)
{
	m_aspect = aspect_ratio;
	++m_revision;
}

void ogs::Camera::SetPosition(glm::vec3 const& position)
{
	m_pos = position;
	++m_revision;
}

glm::mat4 const& ogs::Camera::GetVP() const
{
	if (m_revision > m_lastRevision)
	{
		RecalculateViewProjection();
		m_lastRevision = m_revision;
	}
	return m_vp;
}

void ogs::Camera::RecalculateViewProjection() const
{
	auto const projection = glm::perspective(glm::radians(m_currentFov), m_aspect, 0.1F, 10000.F);
	auto const view = glm::lookAt(m_pos, m_pos + m_forward, glm::vec3(0.0F, 1.0F, 0.0F));
	m_vp = projection * view;
}
