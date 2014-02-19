#include "PieceAggregate.h"

PieceAggregate::PieceAggregate(Piece* v) : Drawable() 
{
	p = v;
	pieces = new std::vector<PieceInstance*>;
	aggID = 0;
}

void PieceAggregate::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye)
{
	p->prepareDraw(viewMatrix, projectionMatrix);
	for (std::vector<PieceInstance*>::iterator it = pieces->begin(); it != pieces->end(); ++it)
	{
		(*it)->draw(viewMatrix, projectionMatrix, cameraEye);
		p->bindDraw();
	}
	p->postDraw();
}

void PieceAggregate::addPiece(PieceInstance* p)
{
	p->setID(aggID++);
	pieces->push_back(p);
}

void PieceAggregate::removePiece(int id)
{
	int numIter = 0;
	for (std::vector<PieceInstance*>::iterator it = pieces->begin(); it != pieces->end(); ++it)
	{
		if ((*it)->getID() == id){
			pieces->erase(pieces->begin() + numIter);
			break;
		}
		numIter++;
	}
}