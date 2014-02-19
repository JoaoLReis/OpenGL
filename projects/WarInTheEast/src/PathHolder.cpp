#include "PathHolder.h"

PathHolder::PathHolder(std::vector<Tile*> p)
{
	path = p;
}

glm::vec3 PathHolder::getPosAtTile(int index)
{
	return path[index]->getPos();
}