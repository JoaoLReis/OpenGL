#pragma once

#include "PieceInstance.h"
#include "Movable.h"

class Enemy : public PieceInstance, public Movable
{

public:
	Enemy(Piece* vp, glm::fquat orientation, glm::mat4 transformation);

	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye);

	void rotate() {};
	void move(float x, float y, float z);
	void handleMovement();

	virtual ~Enemy();
};

