#include "stdafx.h"
#include "Include/Material.h"

void Material::SetParam(PARAM_TYPE type, const Vec3 & vec)
{
	switch (type) {
	case DIFFUSE_COLOR:
		break;
	case AMBIENT_COLOR:
		break;
	case SPECULAR_COLOR:
		break;
	default:
		std::cout << "Wrong type of param" << std::endl;
		return;//TODO: Wyjatek dodac
	}

	if (mVec3Map.count(type) > 0)
	{
		//Swap parameter
		mVec3Map[type] = vec;
	}
	else
	{
		//Add new 
		mVec3Map.insert(std::make_pair(type, vec));
	}

}

void Material::SetParam(PARAM_TYPE type, const float val)
{
	switch (type) {
	case SPEC_POWER:
		break;
	default:
		std::cout << "Wrong type of param" << std::endl;
		return;//TODO: Wyjatek dodac
	}

	if (mFloatMap.count(type) > 0)
	{
		//Swap parameter
		mFloatMap[type] = val;
	}
	else
	{
		//Add new 
		mFloatMap.insert(std::make_pair(type, val));
	}
}

void Material::SetTexture(const Texture &tex)
{
	if (mTextureMap.count(tex.getTextureType()) > 0)
	{
		//Swap parameter
		mTextureMap[tex.getTextureType()] = tex.getTexID();
	}
	else
	{
		//Add new 
		mTextureMap.insert(std::make_pair(tex.getTextureType(), tex.getTexID()));
	}
}

void Material::ApplyParams(ShaderProgram & program)
{
	GLuint shaderFlag = 0;
	for (auto iter = mVec3Map.begin(); iter != mVec3Map.end(); ++iter)
	{
		
		switch (iter->first) {
		case DIFFUSE_COLOR:
			program.apply("Ks", iter->second);
			break;
		case AMBIENT_COLOR:
			program.apply("Kd", iter->second);
			break;
		case SPECULAR_COLOR:
			program.apply("Ka", iter->second);
			break;
		}
	}
	
	for (auto iter = mFloatMap.begin(); iter != mFloatMap.end(); ++iter)
	{

		switch (iter->first) {
		case SPEC_POWER:
			program.apply("spec_power", iter->second);
			break;
		}
	}
	
	for (auto iter = mTextureMap.begin(); iter != mTextureMap.end(); ++iter)
	{
		program.applyTexture(iter->first, iter->second);
		switch (iter->first) {
		case NORMALS_TEXTURE:
			shaderFlag = shaderFlag | NOMRALS_MAP_MODE;
			break;
		case SPECULAR_TEXTURE:
			shaderFlag = shaderFlag | SPECULAR_MAP_MODE;
			break;
		}
	}
	program.apply("shader_flag", shaderFlag);
}
