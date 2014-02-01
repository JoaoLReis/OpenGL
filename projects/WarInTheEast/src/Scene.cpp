#include "Scene.h"

Scene::Scene(std::vector<Piece*>* v)
{
	pieces = v;
	numIds = v->size();
}

void Scene::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraCenter)
{
	for (std::vector<Piece*>::iterator it = pieces->begin() ; it != pieces->end(); ++it)
	{
		//glStencilFunc(GL_ALWAYS, (*it)->getID(), -1);
		(*it)->draw(viewMatrix, projectionMatrix, cameraCenter);
	}
}

void Scene::addPiece(Piece *p)
{
	numIds++;
	p->setId(numIds);
	pieces->push_back(p);
}

Piece* Scene::getPiece(int index)
{
	return pieces->at(index);
}

TileGrid* Scene::getTileGrid()
{
	return (TileGrid*)pieces->at(0);
}

Scene::~Scene()
{
}

