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

void PieceInstance::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye)
{
	handleMovement();
	glm::fquat prevOri = p->getOrientation();
	glm::mat4 prevTrans = p->getTransformation();
	p->setOrientation(orientation);
	p->setTransformation(transformation);
	p->sendModelMatrix(viewMatrix, glm::mat4_cast(orientation) * transformation);
	p->setOrientation(prevOri);
	p->setTransformation(prevTrans);
}

void PieceInstance::handleMovement()
{
	/*Current position of the object.*/
	glm::vec4 currentPos = glm::vec4(0.0, 0.0, 0.0, 1.0);
	currentPos = currentPos * transformation;

	if (moving)
	{
		glm::mat4 x = glm::transpose(glm::translate(glm::mat4(1.0f), 
			glm::vec3(
			Lerp(0.0, objective.x - currentPos.x, variation), 
			Lerp(0.0, objective.y - currentPos.y, variation), 
			Lerp(0.0, objective.z - currentPos.z, variation))));
		transformation = transformation * x;
		if (variation < 1.0)
		{
			variation += 0.00001;
		}
		else
		{
			variation = 0.0;
			moving = false;
		}
	}
}

void PieceInstance::move(float x, float y, float z)
{
	moving = true;
	objective = glm::vec3(x, y, z);
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