#pragma once

#include "../stdafx.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Resource.h"

class Mesh : public Resource{
private:
	Logger * LOGGER = Logger::getLogger();

	unsigned int verticesCount = 0, trianglesCount = 0; 

	GLfloat * points = NULL;
	GLfloat * normals = NULL;
	GLfloat * texcoords = NULL;
	GLfloat * tangents = NULL;

	GLuint vao = 0;

	unsigned int materialIndex = 0;;
	void LoadScene(const aiScene * scene);
public:
	void LoadFile(const std::string &filename);
	void LoadMesh(const aiMesh * mesh);

	void GenBuffers();
	void DeleteBuffers();
	
	unsigned int getMaterialIndex() const;
	Mesh();
	Mesh(const aiMesh * mesh);
	~Mesh();
	int GetVao();
	void Draw();
	void Release();

};

