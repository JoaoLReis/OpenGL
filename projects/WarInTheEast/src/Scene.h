#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Piece.h"
#include "TileGrid.h"
#include "Drawable.h"

class Scene
{
	int globalId;
	std::vector<Drawable*>* pieces;

public:
	Scene(std::vector<Drawable*>* v);
	
	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraCenter);
	void addPiece(Drawable *p);
	void removePiece(int id);

	Drawable* getPiece(int index);

	int getId();

//WARNING!! -> HAS TILEGRID MUST BE THE FIRST IN THE VECTOR
	TileGrid* getTileGrid();

	~Scene();
};

