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
	
	/*Move with a certain speed using linear interpolation.*/
	virtual void move(float x, float y, float z) = 0;
	virtual void rotate() = 0;

	/*Temporary place for interpolation functions.*/
	float Lerp(float a, float b, float var) {
		return  a + (b - a) * var;
	}
};