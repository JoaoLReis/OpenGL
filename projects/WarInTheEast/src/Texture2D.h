#pragma once
#include "Includes.h"
#include "Textures.h"
#include "SOIL\SOIL.h"

class Texture2D : public Texture
{
	 
public:
	Texture2D();
	void load(const std::string& filename);
	void loadmipmap();
	void bind();
	void actBindTexture();
	void createPerlinNoise(int side,double x, double y, double a, double b, int n);

};