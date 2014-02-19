#pragma once
#include "Includes.h"

class Drawable
{
public:
	Drawable(){};
	
	/*Essencial functions that represent what a drawable object is.*/
	virtual void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye) = 0; 
	virtual Drawable* clone() = 0;
	virtual int getID() = 0;
	
};