#pragma once
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <optional>

struct Wavefront_Vertex
{
	int idx_position;
	int idx_texcoord;
	int idx_normal;
};

struct Wavefront_Triangle
{
	Wavefront_Vertex vertices[3];
};

struct Wavefront_File
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<Wavefront_Triangle> triangles;
};

std::optional<Wavefront_File> wavefront_load(std::string const& path);
