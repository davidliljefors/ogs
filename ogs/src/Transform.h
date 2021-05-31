#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ogs {

struct Transform
{
	Transform() = default;
	Transform( glm::vec3 const& position ) : _position( position ) {};

	void Invalidate() { _dirty = true; }

	auto const& Mat4() const
	{
		if ( _dirty )
		{
			_dirty = false;
			_matrix = glm::mat4( 1.0F );
			_matrix = glm::translate( _matrix, _position );
			_matrix = glm::rotate( _matrix, glm::radians( _rotation.x ), glm::vec3( 1.0F, 0.0F, 0.0F ) );
			_matrix = glm::rotate( _matrix, glm::radians( _rotation.y ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
			_matrix = glm::rotate( _matrix, glm::radians( _rotation.z ), glm::vec3( 0.0F, 0.0F, 1.0F ) );
			_matrix = glm::scale( _matrix, _scale );
		}

		return _matrix;
	};

	inline void SetPosition( glm::vec3 const& position )
	{
		Invalidate();
		_position = position;
	}

	inline void SetRotation( glm::vec3 const& rotation )
	{
		Invalidate();
		_rotation = rotation;
	}

	inline void SetScale( glm::vec3 const& scale )
	{
		Invalidate();
		_scale = scale;
	}

	inline glm::vec3 const& GetPosition()
	{
		return _position;
	}

	inline glm::vec3 const& GetRotation()
	{
		return _rotation;
	}

	inline glm::vec3 const& GetScale()
	{
		return _scale;
	}

	//Must call invalidate, for imgui
	inline float* PosPtr()
	{
		return glm::value_ptr( _position );
	}

	inline float* RotPtr()
	{
		return glm::value_ptr( _rotation );
	}

	inline float* ScalePtr()
	{
		return glm::value_ptr( _scale );
	}


private:
	glm::vec3 _position{ 0.0F };
	glm::vec3 _rotation{ 0.0F };
	glm::vec3 _scale{ 1.0F };
	mutable bool _dirty = true;
	mutable glm::mat4 _matrix{ 1.0F };
};

}