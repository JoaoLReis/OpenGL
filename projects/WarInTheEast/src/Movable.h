#pragma once

class Movable
{
public:
	Movable() {};

	/*Move with a certain speed using linear interpolation.*/
	virtual void move(float x, float y, float z) = 0;
	virtual void rotate() = 0;

	/*Temporary place for interpolation functions.*/
	float Lerp(float a, float b, float var) {
		return  a + (b - a) * var;
	}

	virtual ~Movable() {};
};

