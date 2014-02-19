#include "Spawner.h"

Spawner::Spawner(Tile* st, Tile* obj)
{
	starter = st;
	objective = obj;
	std::vector<Tile*> toSearch;
	toSearch.push_back(starter);
	//calculatePath(toSearch);
	A_asterisk(starter, objective);
	std::cout <<"A* search ended" << std::endl;
}

std::map <std::string, Tile*> reconstruct_path(std::map <std::string, Tile*> came_from, Tile* current_node)
{
	std::map <std::string, Tile*> p;
	if (came_from.count(current_node->getPosInString()) == 1)
	{
		p = reconstruct_path(came_from, came_from[current_node->getPosInString()]);
		current_node->setType(VISITED);
		p.insert(std::pair<std::string, Tile*>(current_node->getPosInString(), current_node));
		return p;
	}

	else
	{
		current_node->setType(VISITED);
		p.insert(std::pair<std::string, Tile*>(current_node->getPosInString(), current_node));
		return p;
	}
}

std::map <std::string, Tile*> Spawner::A_asterisk(Tile* start, Tile* goal)
{
	//nodes already evaluated
	std::map <std::string, Tile*> closedset;
	//Nodes to be evaluated filled with the start node
	std::map <std::string, Tile*> openset;
	openset.insert(std::pair<std::string, Tile*>(start->getPosInString(), start));
	//Navigated/Visited nodes
	std::map <std::string, Tile*> came_from;

	//Inicial cost from start
	//start->setHeuristic(0.0);
	//Estimated cost from start to finish
	//start->setf_Heuristic(start->getHeuristic() + getDist2Obj(start->getPos(), goal->getPos()));
	Tile* current;
	float tentative_g_score = 0.0;
	int s = SDL_GetTicks();
	float lowest_heuristic;
	while (!openset.empty())
	{
		lowest_heuristic = 99999;
		for (std::map <std::string, Tile*>::iterator it = openset.begin(); it != openset.end(); ++it)
		{
			//heuristic of the node is calculated here
			it->second->setf_Heuristic(getDist2Obj(it->second->getPos(), goal->getPos()));
			if (it->second->getf_Heuristic() < lowest_heuristic)
			{
				lowest_heuristic = it->second->getf_Heuristic();
				current = it->second;
			}
		}
		//Is the current the goal node?
		if (current->getType() == EXIT)
			return reconstruct_path(came_from, goal);
		
		//Remove current from openset
		openset.erase(current->getPosInString());
		closedset.insert(std::pair<std::string, Tile*>(current->getPosInString(), current));
		std::cout << current->getPos().x << " ; " << current->getPos().y << " ; " << current->getPos().z << std::endl;
		//iterating current neighbors
		for (std::vector<Tile*>::iterator it = current->getAdj()->begin(); it != current->getAdj()->end(); ++it)
		{
			if ((*it)->getType() == MOVEMENT || (*it)->getType() == EXIT)
			{
				if (closedset.count((*it)->getPosInString()) > 0)
					continue;
				tentative_g_score = current->getHeuristic() + getDist2Obj(current->getPos(), (*it)->getPos());

				if (!openset.count((*it)->getPosInString()) > 0 || tentative_g_score < (*it)->getHeuristic())
				{
					/*Sets the previous node to acess when given the current ID to came_from*/
					came_from[(*it)->getPosInString()] = current;
					(*it)->setHeuristic(tentative_g_score);
					(*it)->setf_Heuristic((*it)->getHeuristic() + getDist2Obj((*it)->getPos(), goal->getPos()));
					if (!openset.count((*it)->getPosInString()) > 0)
						openset.insert(std::pair<std::string, Tile*>((*it)->getPosInString(), (*it)));
				}
			}
		}
		int eventtime = SDL_GetTicks() - s;
		if (eventtime >= 200)
			return openset;
	}
}

/*Returns the next tile to the objective 
from the adjacent list of a single tile, 
based on the smalest distance to it.*/
void Spawner::calculatePath(std::vector<Tile*> toSearch)
{
	if (toSearch.empty())
		return;
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