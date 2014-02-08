#include "Scene.h"

Scene::Scene(std::vector<Piece*>* v)
{
	pieces = v;
	globalId = 1;
}

void Scene::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraCenter)
{
	for (std::vector<Piece*>::iterator it = pieces->begin() ; it != pieces->end(); ++it)
	{
		
		(*it)->draw(viewMatrix, projectionMatrix, cameraCenter);
	}
}

void Scene::addPiece(Piece *p)
{
	pieces->push_back(p);
}

void Scene::removePiece(int id)
{
	int numIter = 0;
	for (std::vector<Piece*>::iterator it = pieces->begin(); it != pieces->end(); ++it)
	{
		if ((*it)->getID() == id){
			pieces->erase(pieces->begin() + numIter);
			break;
		}
		numIter++;
	}
}

Piece* Scene::getPiece(int index)
{
	return pieces->at(index);
}

int Scene::getId()
{
	return globalId++;
}

TileGrid* Scene::getTileGrid()
{
	return (TileGrid*)pieces->at(0);
}

Scene::~Scene()
{
}

