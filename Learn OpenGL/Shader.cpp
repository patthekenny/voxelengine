#include "Shader.h"

#define FMT_HEADER_ONLY
#include <fmt/format.h>

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	std::string vertexSource;
	std::string fragmentSource;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	fragmentShaderFile.exceptions(std::fstream::failbit | std::ifstream::badbit);
	vertexShaderFile.exceptions(std::fstream::failbit | std::ifstream::badbit);

	try 
	{
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;
		
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		
		vertexShaderFile.close();
		fragmentShaderFile.close();
		

		vertexSource = vertexShaderStream.str();
		fragmentSource = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		fmt::print("ERROR: CAN'T READ SHADER FILE.");
	}
	
	const char * vertexShaderCode = vertexSource.c_str();
	const char * fragmentShaderCode = fragmentSource.c_str();

	GLuint vertexShader, fragmentShader;

	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fmt::print("ERROR COMPILING VERTEX SHADER \"{}\":\n{}", vertexPath, infoLog);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		fmt::print("ERROR COMPILING VERTEX SHADER \"{}\":\n{}", fragmentPath, infoLog);
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		fmt::print("ERROR LINKING PROGRAM(VERT_FILE={}, FRAG_FILE={}):\n{}", vertexPath, fragmentPath, infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}