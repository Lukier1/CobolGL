#include "stdafx.h"
#include "Include\Mesh.h"

void Mesh::LoadScene(const aiScene * scene)
{
	if (scene->HasMeshes())
	{
		LOGGER->Info("Loaded file contains " + std::to_string(scene->mNumMeshes) + " meshes");
		const aiMesh * mesh = scene->mMeshes[0];
		LoadMesh(mesh);

	}
	else
	{
		LOGGER->Error("Can't load mesh from scene");
	}
}
void Mesh::LoadMesh(const aiMesh * mesh) {
	LOGGER->Info("Loaded meshes with " + std::to_string(mesh->mNumVertices) + " vertices");
	verticesCount = mesh->mNumVertices;
	trianglesCount = mesh->mNumFaces;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	int memInd = 0;
	points = new GLfloat[3 * 3 * mesh->mNumFaces];
	normals = new GLfloat[3 * 3 * mesh->mNumFaces];
	texcoords = new GLfloat[2 * 3 * mesh->mNumFaces];
	verticesCount = 3 * mesh->mNumFaces;

	materialIndex = mesh->mMaterialIndex;

	std::cout << "Material index: " << materialIndex << std::endl;
	for (unsigned int x = 0; x < mesh->mNumFaces; ++x)
	{
		const aiFace face = mesh->mFaces[x];
		for (unsigned int z = 0; z < face.mNumIndices; ++z)
		{
			int vertInd = face.mIndices[z];
			if (mesh->HasPositions())
			{
				const aiVector3D* vp = &(mesh->mVertices[vertInd]);
				points[memInd * 3] = (GLfloat)vp->x;
				points[memInd * 3 + 1] = (GLfloat)vp->y;
				points[memInd * 3 + 2] = (GLfloat)vp->z;
			}
			if (mesh->HasNormals())
			{

				const aiVector3D* vn = &(mesh->mNormals[vertInd]);
				normals[memInd * 3] = (GLfloat)vn->x;
				normals[memInd * 3 + 1] = (GLfloat)vn->y;
				normals[memInd * 3 + 2] = (GLfloat)vn->z;
			}
			if (mesh->HasTextureCoords(0))
			{

				const aiVector3D* vt = &(mesh->mTextureCoords[0][vertInd]);
				texcoords[memInd * 2] = (GLfloat)vt->x;
				texcoords[memInd * 2 + 1] = (GLfloat)vt->y;

			}
			++memInd;
		}
	}

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * verticesCount * sizeof(float), points, GL_STATIC_DRAW);
	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * verticesCount  * sizeof(float), normals, GL_STATIC_DRAW);
	GLuint tc_vbo = 0;
	glGenBuffers(1, &tc_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tc_vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * verticesCount  * sizeof(float), texcoords, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindVertexBuffer(0, vbo, 0, 3 * sizeof(GL_FLOAT));
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(1);
	glBindVertexBuffer(1, colours_vbo, 0, 3 * sizeof(GL_FLOAT));
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);
	glEnableVertexAttribArray(2);
	glBindVertexBuffer(2, tc_vbo, 0, 2 * sizeof(GL_FLOAT));
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

}
void Mesh::GenBuffers()
{ }
void Mesh::DeleteBuffers()
{

}
unsigned int Mesh::getMaterialIndex() const
{
	return materialIndex;
}
Mesh::Mesh()
{
}
Mesh::Mesh(const aiMesh * mesh)
{
	LoadMesh(mesh);
}
void Mesh::LoadFile(const std::string &filename)
	{
		Assimp::Importer importer;

		const aiScene * scene = importer.ReadFile(filename,
			//aiProcess_CalcTangentSpace |
			aiProcess_Triangulate);
		if (!scene) {
			LOGGER->Error("Can't load mesh: " + filename);
		}
		else
		{
			LOGGER->Info("Mesh loading success: " + filename);
		}
		LoadScene(scene);
		//delete scene;
	}

Mesh::~Mesh() {
		Release();
	}
int Mesh::GetVao() {
		return vao;
	}
void Mesh::Draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}
void Mesh::Release() {
	delete[] points;
	points = nullptr;
	delete[] normals;
	normals = nullptr;
	delete[] texcoords;
	texcoords = nullptr;
}



