#pragma once

#include "Includes.h"
#include "Drawable.h"
#include "PieceInstance.h"

class PieceAggregate : public Drawable
{
	Piece* p;
	std::vector<PieceInstance*> *pieces;
	int aggID;

public:
	PieceAggregate(Piece* p);

	virtual void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye); 

	void addPiece(PieceInstance* p);
	void removePiece(int id);
	
	/*Not Implemented*/
	Drawable* clone(){ return 0; };	
	/**/
	void setID(int v){  };
	int getID(){ return 0; }
};