#include "stdafx.h"
#include "Include/ShaderProgram.h"
#include "Include\Exceptions\IOException.h"

ShaderProgram::ShaderProgram() {
	isProgramGenerated = false;
	programId = (GLuint)-1;
	for (int i = 0; i < TYPE_COUNT; ++i)
		shaderId[i] = (GLuint)-1;
}

// throws IOException
void ShaderProgram::AddShader(std::string filename, SHADER_TYPE type) {
	std::string fileText;
	std::fstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(filename, std::fstream::in); // catch error
		fileText = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	catch (std::fstream::failure e)
	{
		Logger::getLogger()->Error("Can't open file: " + filename);
		throw IOException("Can't open shader file: " + filename);
	}

	const char* p_text = fileText.c_str();
	GLuint shaderout = glCreateShader(ResolveShaderId(type));
	glShaderSource(shaderout, 1, &p_text, NULL);
	glCompileShader(shaderout);

	GLint isCompiled = 0;
	glGetShaderiv(shaderout, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderout, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		std::unique_ptr<char> out(new char[maxLength + 1]);
		out.get()[maxLength] = 0;
		glGetShaderInfoLog(shaderout, maxLength, &maxLength, out.get());

		std::string description(out.get());

		Logger::getLogger()->Error("Compilation error file: " + filename + "\nerrors:\n" + description);

		glDeleteShader(shaderout); // Don't leak the shader.
	
	}
	else
	{
		shaderId[type] = shaderout;
	}
}
void ShaderProgram::GenerateProgram() {
	GLuint shader_programme = glCreateProgram();
	for (int i = 0; i < TYPE_COUNT; ++i) {
		if (shaderId[i] != (GLuint)-1) {
			glAttachShader(shader_programme, shaderId[i]);
		}
	}
	glLinkProgram(shader_programme);
	programId = shader_programme;
	Logger::getLogger()->Info("Program compilation succes");

}
bool ShaderProgram::IsProgramGenerated() const {
	return isProgramGenerated;
}

GLuint ShaderProgram::getProgramId() const {
	return programId;
}
GLuint ShaderProgram::glGetShaderId(SHADER_TYPE type) const {
	return shaderId[(int)type];
}

void ShaderProgram::Release() {
	if (programId != -1)
	{
		glDeleteProgram(programId);
		isProgramGenerated = false;
		programId = (GLuint)-1;
	}
	for (int i = 0; i < TYPE_COUNT; ++i) {
		if (shaderId[i] != (GLuint)-1) {
			glDeleteProgram(shaderId[i]);
			shaderId[i] = (GLuint)-1;
		}
	}
}

void ShaderProgram::apply(std::string locator, const Matrix4x4 &mat) {
	GLuint matrix_location = glGetUniformLocation(programId, locator.c_str());
	glUniformMatrix4fv(matrix_location, 1, GL_TRUE, mat.getMatrixData());
}
void ShaderProgram::apply(std::string locator, const Vec3 &vec) {
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform3fv(location, 1, vec.getData());
}
void ShaderProgram::apply(std::string locator, float val) {
	
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform1f(location, val);

}
void ShaderProgram::applyTexture(std::string locator, GLuint texId) {
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform1i(location, texId);
}

void ShaderProgram::UseProgram()
{
	glUseProgram(programId);
}
ShaderProgram::~ShaderProgram() {
	Release();
}

//	static const unsigned short TYPE_COUNT = 2;

GLuint ShaderProgram::ResolveShaderId(SHADER_TYPE type) const {
	switch (type)
	{
	case ShaderProgram::VERTEX:
		return GL_VERTEX_SHADER;
		break;
	case ShaderProgram::FRAGMENT:
		return GL_FRAGMENT_SHADER;
		break;
	default:
		return (GLuint)-1;
		break;
	}
}
