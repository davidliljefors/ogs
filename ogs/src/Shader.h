#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(std::string const& vertex_path, std::string const& fragment_path);
	
	~Shader() 
	{ 
		glDeleteProgram(program); 
	}

	void Bind() const;

	inline void SetFloat3(std::string const& name, glm::vec3 const& value)
	{
		GLint const location = glGetUniformLocation(program, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	inline void SetFloat4(std::string const& name, glm::vec4 const& value)
	{
		GLint const location = glGetUniformLocation(program, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	inline void SetMat4(std::string const& name, glm::mat4 const& value)
	{
		GLint const location = glGetUniformLocation(program, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

private:
	GLuint const program = 0;
};
