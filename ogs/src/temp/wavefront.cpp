#include "wavefront.h"
#include <Core.h>
#include <fstream>


static bool parse_vertex_and_jump(char** str, Wavefront_Vertex* vert)
{
	// nullptr or null character
	if (!(*str) || !(**str))
		return false;

	if (sscanf_s(*str, "%d/%d/%d", &vert->idx_position, &vert->idx_texcoord, &vert->idx_normal) == EOF)
		return false;

	// 1 to 0-based...
	vert->idx_position -= 1;
	vert->idx_texcoord -= 1;
	vert->idx_normal -= 1;

	// Jump to next spacer
	*str = strstr(*str + 1, " ");
	return true;
}

std::unique_ptr<Wavefront_File> wavefront_load(std::string const& path)
{
	// Open file
	std::ifstream file;
	file.open(path);

	if (!file)
	{
		ogs::LogError("Cannot open model '{}'.", path);
		return nullptr;
	}

	Wavefront_File wav;

	// Count positions, texcoords etc...
	int num_positions = 0;
	int num_texcoords = 0;
	int num_normals = 0;
	int num_triangles = 0;

	static char buffer[512];
	while(file.getline(buffer, 512))
	{
		// Position
		if (strncmp(buffer, "v ", 2) == 0)
			num_positions++;
		// Texture coordinate
		else if (strncmp(buffer, "vt", 2) == 0)
			num_texcoords++;
		// Normal
		else if (strncmp(buffer, "vn", 2) == 0)
			num_normals++;

		// Face
		else if (strncmp(buffer, "f ", 2) == 0)
		{
			// Faces are rough, since we need to triangulate for OpenGL
			// So every face with verts > 3 turns into 1 + (verts - 3) triangles (using TRIANGLE_FAN method)
			int verts_in_face = 0;
			char* ptr = buffer + 2;

			Wavefront_Vertex dummy; // :I
			while(parse_vertex_and_jump(&ptr, &dummy))
				verts_in_face++;

			num_triangles += 1 + (verts_in_face - 3);
		}
	}

	// Allocate everything
	wav.positions.reserve(num_positions);
	wav.texcoords.reserve(num_positions);
	wav.normals.reserve(num_positions);
	wav.triangles.reserve(num_triangles);
 
	// Reset, time to parse this puppy!
	num_positions = num_texcoords = num_normals = num_triangles = 0;

	file.clear();
	file.seekg(std::ios_base::beg);

	while(file.getline(buffer, 512))
	{
		// Position
		if (strncmp(buffer, "v ", 2) == 0)
		{
			glm::vec3 pos;
			sscanf_s(buffer, "v %f %f %f", &pos.x, &pos.y, &pos.z);
			wav.positions.push_back(pos);
		}
		// Texture coordinate
		else if (strncmp(buffer, "vt", 2) == 0)
		{
			glm::vec2 texcoord;
			sscanf_s(buffer, "vt %f %f", &texcoord.x, &texcoord.y);
			wav.texcoords.push_back(texcoord);
		}
		// Normal
		else if (strncmp(buffer, "vn", 2) == 0)
		{
			glm::vec3 normal;
			sscanf_s(buffer, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
			wav.normals.push_back(normal);
		}
		// Face
		else if (strncmp(buffer, "f ", 2) == 0)
		{
			char* ptr = buffer + 2;

			// Load two first verts for TRIANGLE_FAN'ning
			Wavefront_Vertex first_vert;
			Wavefront_Vertex prev_vert;

			parse_vertex_and_jump(&ptr, &first_vert);
			parse_vertex_and_jump(&ptr, &prev_vert);

			// Then move forwards one at a time
			Wavefront_Vertex next_vert;
			while(parse_vertex_and_jump(&ptr, &next_vert))
			{
				Wavefront_Triangle tri;
				tri.vertices[0] = first_vert;
				tri.vertices[1] = prev_vert;
				tri.vertices[2] = next_vert;

				wav.triangles.push_back(tri);

				// This vert now becomes the 'prev' vert, but first will stay constant
				prev_vert = next_vert;

				// I swear it took every strength in me to not accidentally write "perv_vert"
				// I'm sorry jesus
			}
		}
	}

	// Done :)
	return std::make_unique<Wavefront_File>(wav);
}
