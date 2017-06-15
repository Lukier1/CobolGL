#pragma once

#include <map>
#include "CobMath.h"
#include "Texture.h"
#include "ShaderProgram.h"
class Material {
public:
	enum PARAM_TYPE {
		DIFFUSE_COLOR,
		AMBIENT_COLOR,
		SPECULAR_COLOR,
		SPEC_POWER
	};

	void SetParam(PARAM_TYPE type, const Vec3 &vec);
	void SetParam(PARAM_TYPE type, const float val);

	void SetTexture(const Texture &tex);

	void ApplyParams(ShaderProgram &program);
private:
	std::map<PARAM_TYPE, Vec3> mVec3Map;
	std::map<PARAM_TYPE, float> mFloatMap;
	std::map<TextureType, GLuint> mTextureMap;
};