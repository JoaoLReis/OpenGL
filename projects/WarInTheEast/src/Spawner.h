#pragma once
#include "Includes.h"
#include "Tile.h"

class Spawner
{
	/*Level of mobs to spawn.*/
	int level;
	Tile *starter, *objective;
	std::vector<Tile*> path;
	
public:
	Spawner(Tile* starter, Tile* Objective);
	
	/*Calculates the path to the objective.*/
	void calculatePath(std::vector<Tile*> toSearch);
	float getDist2Obj(glm::vec3 start, glm::vec3 finish);
	void changeObjective(Tile* Objective);
};