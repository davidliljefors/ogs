#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ogs {

struct Transform
{
	Transform() = default;

	void Invalidate() { _dirty = true; }

	auto const& Mat4() const
	{
		if (_dirty)
		{
			_dirty = false;
			_matrix =  glm::mat4(1.0F);
			_matrix = glm::translate(_matrix, Position);
			_matrix = glm::rotate(_matrix, glm::radians(Rotation.x), glm::vec3(1.0F, 0.0F, 0.0F));
			_matrix = glm::rotate(_matrix, glm::radians(Rotation.y), glm::vec3(0.0F, 1.0F, 0.0F));
			_matrix = glm::rotate(_matrix, glm::radians(Rotation.z), glm::vec3(0.0F, 0.0F, 1.0F));
			_matrix = glm::scale(_matrix, Scale);
		}

		return _matrix;
	};

	glm::vec3 Position {0.0F};
	glm::vec3 Rotation {0.0F};
	glm::vec3 Scale { 1.0F };
private:
	mutable glm::mat4 _matrix;
	mutable bool _dirty = true;
};

}