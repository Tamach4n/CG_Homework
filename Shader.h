#pragma once
#include <string>
#include <gl/glew.h>

class Shader
{
public:
	Shader();

	~Shader();

	bool Load(const std::string& vertName, const std::string& fragName);

	void Unload();

	void setActive();

	void setUniform(const char* name, float xPos, float yPos);
	void setUniform4(const char* name, float x1, float y1, float x2, float y2);

	GLuint GetshaderProgram() const { return shaderProgram; }

private:
	bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	bool isCompiled(GLuint shader);

	bool isValidProgram();

	GLuint vertexShader;
	GLuint fragShader;
	GLuint shaderProgram;
};

