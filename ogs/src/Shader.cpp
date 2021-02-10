#include <glad/glad.h>

#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>

#include "Shader.h"

Shader::Shader(std::string const& vertex_path, std::string const& fragment_path)
	: program(glCreateProgram())
{
	auto read_file = [](std::string const& file_path) {
		std::ifstream ifs(file_path);
		std::ostringstream os;
		os << ifs.rdbuf();
		return os.str();
	};

	auto compile_shader = [](std::string const& shader_source, int shader_type)
	{
		GLchar const* vert_cstr = shader_source.c_str();
		GLuint const shader = glCreateShader(shader_type);
		glShaderSource(shader, 1, &vert_cstr, 0);
		glCompileShader(shader);

		int compile_result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);
		if (compile_result == GL_FALSE)
		{
			int err_log_len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &err_log_len);

			auto const error_log = std::make_unique<char[]>(err_log_len);
			glGetShaderInfoLog(shader, err_log_len, &err_log_len, error_log.get());
			std::cout << "Failed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
			std::cerr << error_log.get();
		}
		return shader;
	};

	auto const frag_shader = compile_shader(read_file(fragment_path), GL_FRAGMENT_SHADER);
	auto const vert_shader = compile_shader(read_file(vertex_path), GL_VERTEX_SHADER);

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(frag_shader);
	glDeleteShader(vert_shader);
}

void Shader::Bind() const
{
	glUseProgram(program);
}
