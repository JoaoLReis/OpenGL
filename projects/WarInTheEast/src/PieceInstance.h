#pragma once

#include "Includes.h"
#include "Drawable.h"
#include "Piece.h"

class PieceInstance : public Drawable
{
	Piece* p;
	glm::fquat orientation;
	glm::mat4 transformation;
	/*Variation on the current move towards the objective.*/
	float variation;
	/*Objective to move towards, changed by the method move.*/
	glm::vec3 objective;
	/*Boolean to check if piece is moving*/
	bool moving;
	int id;

public:
	PieceInstance(Piece* vp, glm::fquat orientation, glm::mat4 transformation);

	virtual void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye); 

	/*Not Implemented*/
	Drawable* clone(){ return 0; };	
	void rotate() {};
	/**/
	void setID(int v);
	int getID();

	void move(float x, float y, float z);
	void handleMovement();

	void translate(glm::vec3 vec);
	void reset();
};