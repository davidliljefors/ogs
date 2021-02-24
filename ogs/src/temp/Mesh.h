#pragma once
#include <vector>

#include "VertexArray.h"
#include "wavefront.h"

namespace ogs {

class Mesh {
	friend class Renderer;

public:
	Mesh(Wavefront_File const& obj);
	
private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;
	VertexArray _vao;
};

}