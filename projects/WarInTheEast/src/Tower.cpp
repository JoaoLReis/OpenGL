#include "Tower.h"


Tower::Tower(Piece* vp, glm::fquat orientation, glm::mat4 transformation) : PieceInstance(vp, orientation, transformation)
{
}

void Tower::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye)
{
	glm::fquat prevOri = p->getOrientation();
	glm::mat4 prevTrans = p->getTransformation();
	p->setOrientation(orientation);
	p->setTransformation(transformation);
	p->sendModelMatrix(viewMatrix, glm::mat4_cast(orientation) * transformation);
	p->setOrientation(prevOri);
	p->setTransformation(prevTrans);
}

Tower::~Tower()
{
}
