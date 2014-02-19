#pragma once
#include "Includes.h"
#include "Tile.h"

class PathHolder
{
private:
	std::vector<Tile*> path;

public:
	PathHolder(std::vector<Tile*> p);	
	glm::vec3 getPosAtTile(int index);
};