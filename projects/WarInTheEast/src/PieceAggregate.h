#pragma once

#include "Includes.h"
#include "Drawable.h"
#include "PieceInstance.h"

class PieceAggregate : public Drawable
{
	Piece* p;
	std::vector<PieceInstance*> *pieces;

public:
	PieceAggregate(Piece* p);

	virtual void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye); 

	void addPiece(PieceInstance* p);
	
	/*Not Implemented*/
	Drawable* clone(){ return 0; };	
	void move(float x, float y, float z) {};
	void rotate() {};
	/**/
	void setID(int v){  };
	int getID(){ return 0; }
};