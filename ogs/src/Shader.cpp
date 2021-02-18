#include <glad/glad.h>

#include <sstream>
#include <fstream>
#include <memory>

#include "Core.h"

#include "Shader.h"

ogs::Shader::Shader(std::string const& vertex_path, 
					std::string const& fragment_path)
	: program(glCreateProgram())
{
	auto ReadFile = [](std::string const& file_path) {
		std::ifstream ifs(file_path);
		std::ostringstream os;
		os << ifs.rdbuf();
		return os.str();
	};

	auto CompileShader = [](std::string const& shader_source, int shader_type)
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
			LogError("Failed to compile {} shader", (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
			LogError(error_log.get());
		}
		return shader;
	};

	auto const frag_shader = CompileShader(ReadFile(fragment_path), GL_FRAGMENT_SHADER);
	auto const vert_shader = CompileShader(ReadFile(vertex_path), GL_VERTEX_SHADER);

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);

	glDeleteShader(frag_shader);
	glDeleteShader(vert_shader);
}

ogs::Shader::Shader(Shader&& shader) noexcept
{
	std::swap(program, shader.program);
	uniform_location_cache = std::move(shader.uniform_location_cache);
}

ogs::Shader& ogs::Shader::operator=(Shader&& rhs) noexcept
{
	std::swap(program, rhs.program);
	uniform_location_cache = std::move(rhs.uniform_location_cache);

	return *this;
}

ogs::Shader::~Shader()
{
	if (program)
	{
		glDeleteProgram(program);
	}
}

void ogs::Shader::Bind() const
{
	glUseProgram(program);
}

unsigned int ogs::Shader::GetUniformLocation(std::string const& name) const
{
	if (uniform_location_cache.contains(name))
	{
		return uniform_location_cache[name];
	}

	auto const location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
	{
		LogWarning("Warning: uniform '{}' doesn't exist!", name);
	}
	
	uniform_location_cache[name] = location;
	return location;
}
