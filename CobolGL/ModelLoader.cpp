#include "stdafx.h"
#include "Include\ModelLoader.h"
#include "Include\Mesh.h"

#include "Include\Exceptions\MissingResourceException.h"

ModelLoader::ModelLoader()
{
}

ModelLoader::ModelLoader(const std::string & filename)
{
	LoadFile(filename);
}

ModelLoader::~ModelLoader()
{
	for (Material * material : mMateriaList)
	{
		delete material;
	}
	for (Mesh * mesh : mMeshesList)
	{
		delete mesh;
	}
}

void ModelLoader::LoadFile(const std::string & filename)
{
	Assimp::Importer importer;

	const aiScene * scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace);
	
	if (!scene) {
		LOGGER->Error("Can't load mesh: " + filename);
	}
	else
	{
		LOGGER->Info("Mesh loading success: " + filename);
		mFilename = filename;
	}
	
	
	//Test properties
	if (scene->HasMaterials())
	{
		for (unsigned int iMat = 0; iMat < scene->mNumMaterials; ++iMat)
		{
			aiMaterial * material = scene->mMaterials[iMat];
			Material * newMaterial = new Material();
			LOGGER->Info("New Material #####################");
			for (unsigned int z = 0; z < material->mNumProperties; ++z)
			{
				std::string out_data = "";
				const aiMaterialProperty * prop = material->mProperties[z];
				if (prop->mDataLength > 0) {
					std::string key(prop->mKey.data);
					const unsigned int dataLenght = prop->mDataLength;
					switch (prop->mType)
					{
					case aiPTI_Float: 
					{
						float * dataConv = (float*)prop->mData;
						if (dataLenght == 4) // Float 
						{
							float val = dataConv[0];
							out_data = std::to_string(dataConv[0]);
							if (key == "$mat.shininess")
							{
								newMaterial->SetParam(Material::SPEC_POWER, val);
							}
						}
						else if (dataLenght == 8) // Vec2
						{
							Vec2 vec;// (dataConv[0], dataConv[1], dataConv[2]);
							vec.v[0] = dataConv[0];
							vec.v[1] = dataConv[1];
							out_data = std::to_string(dataConv[0]);
							out_data += " " + std::to_string(dataConv[1]);
						}
						else if (dataLenght == 12) { // Vec3
							Vec3 vec = Vector3f(dataConv[0], dataConv[1], dataConv[2]);
							if (key == "$clr.ambient")
							{
								newMaterial->SetParam(Material::AMBIENT_COLOR, vec);
							}
							if (key == "$clr.diffuse")
							{
								newMaterial->SetParam(Material::DIFFUSE_COLOR, vec);
							}
							if (key == "$clr.specular")
							{
								newMaterial->SetParam(Material::SPECULAR_COLOR, vec);
							}
							out_data = std::to_string(dataConv[0]);
							out_data += " " + std::to_string(dataConv[1]);
							out_data += " " + std::to_string(dataConv[2]);
						}
					}
						break;
					case aiPTI_String:
						out_data = std::string(prop->mData);
						break;
					case aiPTI_Integer:
						if (dataLenght >= 4)
						{
							out_data = std::to_string(*((int*)prop->mData));
						}
						break;
					}
				}
				LOGGER->Info(std::string(prop->mKey.data) +  " : " + out_data);
			}

			LoadTexture(material, aiTextureType_DIFFUSE, DIFFUSE_TEXTURE, newMaterial, "diffuse");
			LoadTexture(material, aiTextureType_SPECULAR, SPECULAR_TEXTURE, newMaterial, "specular");
			LoadTexture(material, aiTextureType_HEIGHT, NORMALS_TEXTURE, newMaterial, "normals");

			mMateriaList.push_back(newMaterial);
		}
	}
	/////---------------------
	
	
	LoadScene(scene);
	//delete scene;
}

std::vector<Mesh>::const_iterator ModelLoader::getMeshIterator()
{
	return std::vector<Mesh>::const_iterator();
}

const std::vector<Mesh*> ModelLoader::getMeshList()
{
	// TODO: insert return statement here
	return mMeshesList;
}

const std::vector<Material*> ModelLoader::getMaterialList()
{
	return mMateriaList;
}

void ModelLoader::LoadScene(const aiScene * scene)
{
	if (scene->HasMeshes())
	{
		LOGGER->Info("Loaded file contains " + std::to_string(scene->mNumMeshes) + " meshes");
		
		//Loop for all meshes saved in scene
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			LOGGER->Info("Loading mesh: " + std::to_string(i));

			const aiMesh * mesh = scene->mMeshes[i];
			
			mMeshesList.push_back(new Mesh(mesh));
		}
	}
	else
	{
		LOGGER->Error("Can't load mesh from scene");
	}
}

void ModelLoader::LoadTexture(const aiMaterial * material, aiTextureType texType, TextureType outTexType, Material * newMaterial, std::string texTypeName)
{
	if (material->GetTextureCount(texType))
	{
		LOGGER->Info("Material has " + texTypeName + " texture ");
		aiString path;
		material->GetTexture(texType, 0, &path);
		LOGGER->Info(path.data);
		try {
			Texture * tex = new Texture(std::string(path.data), outTexType);
			mTexList.push_back(tex);
			newMaterial->SetTexture(*tex);
		}
		catch (MissingResourceException e)
		{
			LOGGER->Error("Loading model: " + mFilename + " Can't load " + texTypeName + " texture. ");
		}
	}
}
