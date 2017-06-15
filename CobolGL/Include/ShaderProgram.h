#pragma once

#include "Resource.h"
#include "Math\Matrix.h"
#include "Math\Vec.h"
#include "TextureType.h"

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
	
	void UseProgram();
	GLuint getProgramId() const;
	GLuint glGetShaderId(SHADER_TYPE type) const;

	void Release();
	~ShaderProgram();

	void apply(std::string locator, const Matrix4x4 &mat);
	void apply(std::string locator, const Vec3 &vec);
	void apply(std::string locator, float val);
	void applyTexture(GLuint texNum, GLuint texId);
	void applyTexture(TextureType texType, GLuint texId);
private:
	static const unsigned short TYPE_COUNT = 2;

	bool isProgramGenerated;
	GLuint programId;
	GLuint shaderId[TYPE_COUNT];
	std::string shaderFilename[TYPE_COUNT];

	GLuint ResolveShaderId(SHADER_TYPE type) const;
};