#include "EZGL/Shader.h"
#include "Logger.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <new>
#include <fstream>

namespace ezgl
{
	Shader::Shader()
	{
		this->_programId = 0;
	}

	Shader::Shader(const Shader& other)
	{
		*this = other;
	}

	Shader& Shader::operator=(const Shader& other)
	{
		this->_programId = other._programId;
		return *this;
	}

	Shader::Shader(const std::string& vertexPath, const std::string& fragPath)
	{
		this->loadFiles(vertexPath, fragPath);
	}

	void Shader::loadFiles(const std::string& vertexPath, const std::string& fragPath)
	{
		std::ifstream vertexFile(vertexPath);
		std::ifstream fragFile(fragPath);
		if (!vertexFile.good() || !vertexFile.is_open())
			throw std::runtime_error("Could not open the file " + vertexPath);
		if (!fragFile.good() || !fragFile.is_open())
			throw std::runtime_error("Could not open the file " + fragPath);

		std::string vertexSource((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
		std::string fragSource((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());

		this->_programId = glCreateProgram();
		unsigned int vertexId = this->_compileShader(GL_VERTEX_SHADER, vertexSource);
		unsigned int fragId = this->_compileShader(GL_FRAGMENT_SHADER, fragSource);

		glAttachShader(this->_programId, vertexId);
		glAttachShader(this->_programId, fragId);
		glLinkProgram(this->_programId);
		glValidateProgram(this->_programId);

		glDeleteShader(vertexId);
		glDeleteShader(fragId);
	}

	unsigned int Shader::_compileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* str = source.c_str();
		glShaderSource(id, 1, &str, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_malloca(length * sizeof(char));
			if (!message)
				throw std::bad_alloc();

			glGetShaderInfoLog(id, length, &length, message);
			Logger::log("ShaderCompiler", message, Logger::MessageType::Error);
			// throw std::invalid_argument(message);
		}

		return id;
	}

	void Shader::sendUniform(const std::string& name, const Matrix<float, 4, 4>& data) const
	{
		MatrixData<float, 4> matrixData(data);
		
		GLint location = glGetUniformLocation(this->_programId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, matrixData.data);
	}

	void Shader::sendUniform(const std::string& name, const Light& light) const
	{
		this->sendUniform(name + ".direction", light.direction);
		this->sendUniform(name + ".color", light.color);
		this->sendUniform(name + ".intensity", light.intensity);
	}

	void Shader::sendUniform(const std::string& name, const Material& material) const
	{
		this->sendUniform(name + ".color", material.color);
		this->sendUniform(name + ".ambient", material.ambient);
		this->sendUniform(name + ".diffuse", material.diffuse);
		this->sendUniform(name + ".specular", material.specular);
		this->sendUniform(name + ".shininess", material.shininess);
	}

	void Shader::sendUniform(const std::string& name, const Vector3<float>& vec) const
	{
		GLint location = glGetUniformLocation(this->_programId, name.data());
		glUniform3f(location, vec.x, vec.y, vec.z);
	}

	void Shader::sendUniform(const std::string& name, float value) const
	{
		GLint location = glGetUniformLocation(this->_programId, name.data());
		glUniform1f(location, value);
	}

	void Shader::bind() const
	{
		glUseProgram(this->_programId);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}

	int Shader::getProgramId() const
	{
		return this->_programId;
	}
}
