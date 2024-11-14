#include "Shader.h"
#include <iostream>
#include <fstream>	
#include <sstream>

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	if (!compileShader(vertName, GL_VERTEX_SHADER, vertexShader) || !compileShader(fragName, GL_FRAGMENT_SHADER, fragShader))
		return false;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	//glDeleteShader(vertexShader);
	//glDeleteShader(fragShader);

	if (!isValidProgram())
		return false;

	return true;
}

void Shader::Unload()
{
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void Shader::setActive()
{
	glUseProgram(shaderProgram);
}

void Shader::setUniform(const char* name, float xPos, float yPos)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	glUniform2f(loc, xPos, yPos);

	//glUniformMatrix4fv(loc, 1, GL_TRUE, reinterpret_cast<const float*>(&matrix[0][0]));
}

void Shader::setUniform4(const char* name, float x1, float y1, float x2, float y2)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	glUniform4f(loc, x1, y1, x2, y2);
}

bool Shader::compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	std::ifstream shaderFile(fileName);

	if (!shaderFile.is_open()) {
		std::cerr << "Shader not Found: " << fileName.c_str() << '\n';
		return false;
	}

	std::stringstream sstream;
	sstream << shaderFile.rdbuf();

	std::string contents = sstream.str();
	const char* contentsChar = contents.c_str();

	outShader = glCreateShader(shaderType);

	glShaderSource(outShader, 1, &(contentsChar), nullptr);
	glCompileShader(outShader);

	if (!isCompiled(outShader)) {
		std::cerr << "Failde to compile Shader " << fileName.c_str() << '\n';
		return false;
	}

	return true;
}

bool Shader::isCompiled(GLuint shader)
{
	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		GLchar errorLog[512];
		memset(errorLog, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, errorLog);
		std::cerr << "GLSL Compile Failed: " << errorLog << '\n';
		return false;
	}

	return true;
}

bool Shader::isValidProgram()
{
	GLint status;

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

	if (status != GL_TRUE) {
		GLchar errorLog[512];
		memset(errorLog, 0, 512);
		glGetProgramInfoLog(shaderProgram, 511, nullptr, errorLog);
		std::cerr << "GLSL Link status: " << errorLog << '\n';
		return false;
	}

	return true;
}
