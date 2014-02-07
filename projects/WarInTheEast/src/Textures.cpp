#include "Textures.h"

Texture::Texture() : texture(){}

std::string Texture::getTexPath()
{
	return texPath;
}

GLenum Texture::getColorType()
{
	return colorType;
}