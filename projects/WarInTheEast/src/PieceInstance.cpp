#include "PieceInstance.h"

PieceInstance::PieceInstance(Piece* vp, glm::fquat ori, glm::mat4 trans) : Drawable()
{
	p = vp;
	orientation = ori;
	transformation = trans;
	int id;
	variation = 0.0;
	moving = false;
}

void PieceInstance::translate(glm::vec3 vec)
{ 
	transformation = glm::transpose(glm::translate(transformation, vec));
}

void PieceInstance::reset()
{ 
	transformation = glm::mat4(1.0f);
}

void PieceInstance::setID(int v)
{
	id = v;
}

int PieceInstance::getID() 
{
	return id;
}