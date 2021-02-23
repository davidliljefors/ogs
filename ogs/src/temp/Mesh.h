#pragma once

#include <glm/glm.hpp>
#include <string>

#include "VertexArray.h"
#include "wavefront.h"

namespace ogs {
class Texture;

class Mesh {
	friend class Renderer;

public:
	Mesh(Wavefront_File const& obj)
	{
		_vertices.reserve(obj.triangles.size() * 3);

		for (auto& triangle : obj.triangles)
		{
			for (auto& vert : triangle.vertices)
			{
				_indices.push_back(vert.idx_position);
				_vertices.emplace_back(
					obj.positions[vert.idx_position], 
					obj.texcoords[vert.idx_texcoord], 
					obj.normals[vert.idx_normal]);
			}
		}

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		_vao = VertexArray(_vertices, layout);
	}
private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;
	std::vector<Texture*> _textures;
	VertexArray _vao;
};

}