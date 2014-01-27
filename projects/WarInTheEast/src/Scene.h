#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Piece.h"

class Scene
{
	std::vector<Piece*>* pieces;

public:
	Scene(std::vector<Piece*>* v);
	
	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraCenter);

	void addPiece(Piece *p)
	{
		pieces->push_back(p);
	}

	~Scene();
};

