#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Piece.h"
#include "TileGrid.h"

class Scene
{
	int numIds;
	std::vector<Piece*>* pieces;

public:
	Scene(std::vector<Piece*>* v);
	
	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraCenter);

	void addPiece(Piece *p);

	Piece* getPiece(int index);

//WARNING!! -> HAS TILEGRID MUST BE THE FIRST IN THE VECTOR
	TileGrid* getTileGrid();

	~Scene();
};

