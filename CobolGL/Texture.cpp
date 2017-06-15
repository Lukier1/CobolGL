#include "stdafx.h"
#include "include/Texture.h"
#include "Include\Exceptions\MissingResourceException.h"


Texture::Texture(std::string filename, TextureType type)
{
	mType = type;

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP * dib(0);

	fif = FreeImage_GetFileType(filename.c_str());
	dib = FreeImage_Load(fif, filename.c_str());
	if (!dib) {
		Logger::getLogger()->Error("Can't load " + filename);
		throw new MissingResourceException("TEXTURE", filename);
	}
	unsigned char* data = FreeImage_GetBits(dib);
	mImageData = std::shared_ptr<unsigned char>(data);
	W = FreeImage_GetWidth(dib);
	H = FreeImage_GetHeight(dib);

	mTex = 0;
	glGenTextures(1, &mTex);
	//Dobrac za pomoca parametru type
	switch (type) {
	case TextureType::DIFFUSE_TEXTURE:
		glActiveTexture(GL_TEXTURE0);
		break;
	case TextureType::SPECULAR_TEXTURE:
		glActiveTexture(GL_TEXTURE1);
		break;
	}
	glBindTexture(GL_TEXTURE_2D, mTex);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		W, H, 0, GL_BGRA, GL_UNSIGNED_BYTE, mImageData.get());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	
}

Texture::Texture(const Texture & t)
{
	mType = t.mType;
	mTex = t.mTex;
	W = t.W;
	H = t.H;
}

TextureType Texture::getTextureType() const
{
	return mType;
}

GLuint Texture::getTexID() const
{
	return mTex;
}

void Texture::Release()
{
}

