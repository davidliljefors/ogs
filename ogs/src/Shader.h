#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ogs {

class Shader {

public:
	Shader(std::string const& vertex_path, std::string const& fragment_path);

	Shader(Shader const&) = delete;
	Shader& operator=(Shader const&) = delete;

	Shader(Shader&& shader) noexcept;
	Shader& operator=(Shader&& rhs) noexcept;

	~Shader();

	void Bind() const;

	inline void SetInt(std::string const& name, int value) const
	{
		auto const location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	inline void SetFloat(std::string const& name, float value) const
	{
		auto const location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	inline void SetFloat3(std::string const& name, glm::vec3 const& value) const
	{
		auto const location = GetUniformLocation(name);
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	inline void SetFloat4(std::string const& name, glm::vec4 const& value) const
	{
		auto const location = GetUniformLocation(name);
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	inline void SetMat4(std::string const& name, glm::mat4 const& value) const
	{
		auto const location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

private:
	unsigned int GetUniformLocation(std::string const& name) const;

private:
	unsigned int program = 0;
	mutable std::unordered_map<std::string, unsigned int> uniform_location_cache;
};
}
