#pragma once
#include "../stdafx.h"

#include "Mesh.h"
#include "Material.h"
class ModelLoader {
public:
	ModelLoader();
	ModelLoader(const std::string &filename);

	~ModelLoader();

	void LoadFile(const std::string &filename);

	std::vector<Mesh>::const_iterator getMeshIterator(); 
	const std::vector<Mesh*> getMeshList();
	const std::vector<Material*> getMaterialList();
private:
	Logger * LOGGER = Logger::getLogger();
	
	void LoadScene(const aiScene * scene);
	std::vector<Mesh*> mMeshesList;
	std::vector<Material*> mMateriaList;
	std::vector<Texture*> mTexList;
};