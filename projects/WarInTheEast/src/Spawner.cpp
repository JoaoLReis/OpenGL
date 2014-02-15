#include "Spawner.h"

Spawner::Spawner(Tile* st, Tile* obj)
{
	starter = st;
	objective = obj;
	std::vector<Tile*> toSearch;
	toSearch.push_back(starter);
	calculatePath(toSearch);
}

/*Returns the next tile to the objective 
from the adjacent list of a single tile, 
based on the smalest distance to it.*/
void Spawner::calculatePath(std::vector<Tile*> toSearch)
{
	float heuristic = 9999, tmp = 0;
	Tile* chosen = toSearch.at(0);
	Tile* starter = toSearch.at(0);
	toSearch.erase(toSearch.begin());
	std::vector<Tile*> tiles2Add;

	for (std::vector<Tile*>::iterator it = starter->getAdj()->begin(); it != starter->getAdj()->end(); ++it)
	{
		if ((*it)->getType() == MOVEMENT )
		{
			tmp = getDist2Obj((*it)->getPos(), objective->getPos());
			(*it)->setHeuristic(tmp);
			if (tmp <= heuristic)
			{
				heuristic = tmp;
				chosen = (*it);
			}
		}
		else if ((*it)->getType() == EXIT)
		{
			std::cout << "EXIT FOUND YAY" << std::endl;
			return;
		}
	}
	for (std::vector<Tile*>::iterator it = starter->getAdj()->begin(); it != starter->getAdj()->end(); ++it)
	{
		if((*it)->getType() == MOVEMENT)
		{
			if ((*it)->getPos().x != chosen->getPos().x ||
				(*it)->getPos().y != chosen->getPos().y ||
				(*it)->getPos().z != chosen->getPos().z)
			{
				toSearch.push_back((*it));
				//(*it)->setType(VISITED);
			}
		}
	}
	if (chosen->getType() != VISITED)
		toSearch.emplace(toSearch.begin(), chosen);
	chosen->setType(VISITED);
	std::cout << "VISITED-> " << chosen->getHeuristic() << std::endl;
	path.push_back(chosen);
	calculatePath(toSearch);
}

/*Heuristic calculator, returns the distance from start to finish.*/
float Spawner::getDist2Obj(glm::vec3 start, glm::vec3 finish)
{
	glm::vec3 dist = finish - start;
	float h = std::sqrt(dist.x * dist.x + dist.y * dist.y);
	float h2 = std::sqrt(h*h + dist.z * dist.z);
	return h2;
}

void Spawner::changeObjective(Tile* obj)
{
	std::vector<Tile*> toSearch;
	toSearch.push_back(starter);
	objective = obj;
	calculatePath(toSearch);
}