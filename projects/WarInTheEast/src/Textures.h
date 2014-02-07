#pragma once
#include "Includes.h"
#include "SOIL\SOIL.h"

class Texture
{
	GLenum type;
protected:
	std::string texPath;
	GLuint *texture;
	int width, height;
	unsigned char* image;
	GLenum rgbType, colorType;

public:
	Texture();
	virtual void load(const std::string& filename) = 0;
	void bind();
	void unbind();
	virtual void actBindTexture() = 0;

	GLuint *getTexture(){ return texture;}
	GLenum getType() { return type;}
	unsigned char* getImageData() { return image;}
	std::string getTexPath();
	GLenum getColorType();

	void setType(GLenum t) { type = t;}
	void setTexture(GLuint *i){ texture = i;}
};