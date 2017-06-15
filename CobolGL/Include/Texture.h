#pragma once

#include "../stdafx.h"
#include <assimp\scene.h>
#include "Resource.h"
#include "Include\TextureType.h"

class Texture : public Resource {
private:
	GLuint mTex = 0;
	TextureType mType;
	std::shared_ptr<unsigned char> mImageData;
	int W, H;
public:
	Texture(std::string filename, TextureType type);
	Texture(const Texture& t);

	TextureType getTextureType() const;
	GLuint getTexID() const;
	void Release();
};