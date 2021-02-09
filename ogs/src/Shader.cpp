#include <glad/glad.h>

#include <sstream>
#include <fstream>
#include <iostream>

#include "Shader.h"

Shader::Shader(std::string const& vertex_path, std::string const& fragment_path)
	: program(glCreateProgram())
{
	auto read_file = [](std::string const& file_path) {
		std::ifstream ifs(file_path.c_str());
		std::ostringstream os;
		os << ifs.rdbuf();
		return os.str();
	};

	constexpr auto err_buff_size = 1024;
	GLchar error_buffer[err_buff_size];

	// Vert shader
	auto const vert_source = read_file(vertex_path);

	GLchar const* vert_cstr = vert_source.c_str();
	GLuint const vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vert_cstr, 0);
	glCompileShader(vert_shader);

	glGetShaderInfoLog(vert_shader, err_buff_size, NULL, error_buffer);
	std::cout << "Vertex Shader Compile\n";
	std::cerr << error_buffer;

	// Frag shader
	auto const frag_source = read_file(fragment_path);

	GLchar const* frag_cstr = frag_source.c_str();
	GLuint const frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &frag_cstr, 0);
	glCompileShader(frag_shader);


	glGetShaderInfoLog(frag_shader, err_buff_size, NULL, error_buffer);
	std::cout << "Fragment Shader Compile\n";
	std::cerr << error_buffer;

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
}

void Shader::Bind() const
{
	glUseProgram(program);
}
