#pragma once

#include "Resource.h"

class ShaderProgram : public Resource
{
public:
	enum SHADER_TYPE {
		VERTEX = 0,
		FRAGMENT = 1
	};

	ShaderProgram();

	//@throws IOException
	void AddShader(std::string filename, SHADER_TYPE type);
	void GenerateProgram();
	bool IsProgramGenerated() const;
	
	GLuint getProgramId() const;
	GLuint glGetShaderId(SHADER_TYPE type) const;

	void Release();
	~ShaderProgram();
private:
	static const unsigned short TYPE_COUNT = 2;

	bool isProgramGenerated;
	GLuint programId;
	GLuint shaderId[TYPE_COUNT];
	std::string shaderFilename[TYPE_COUNT];

	GLuint ResolveShaderId(SHADER_TYPE type) const;
};