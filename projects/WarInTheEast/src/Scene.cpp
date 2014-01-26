#include "Scene.h"

Scene::Scene(std::vector<Piece*>* v)
{
	pieces = v;
}

void Scene::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraCenter)
{
	for (std::vector<Piece*>::iterator it = pieces->begin() ; it != pieces->end(); ++it)
	{
		(*it)->draw(viewMatrix, projectionMatrix, cameraCenter);
	}
}

Scene::~Scene()
{
}

