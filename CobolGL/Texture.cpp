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
		throw MissingResourceException("TEXTURE", filename);
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
	case TextureType::NORMALS_TEXTURE:
		glActiveTexture(GL_TEXTURE2);
		break;
	}
	glBindTexture(GL_TEXTURE_2D, mTex);

	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(dib);

	if (FreeImage_GetBPP(dib) == 32) {
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA,
			W, H, 0, GL_BGRA, GL_UNSIGNED_BYTE, mImageData.get());
	}
	else if (FreeImage_GetBPP(dib) == 24) {
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB,
			W, H, 0, GL_BGR, GL_UNSIGNED_BYTE, mImageData.get());
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// work out maximum an-isotropy
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

	glGenerateMipmap(GL_TEXTURE_2D);

	Logger::getLogger()->Info("Loaded texture " + filename);
	
	FreeImage_Unload(dib);
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

