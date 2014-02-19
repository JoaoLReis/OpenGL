#pragma once

#include "PieceInstance.h"

class Tower : public PieceInstance
{

public:
	Tower(Piece* vp, glm::fquat orientation, glm::mat4 transformation);

	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye);


	virtual ~Tower();
};

