#pragma once
#include "Piece.h"
#include "Tile.h"

class TileGrid : public Piece
{
	std::vector<Tile*> tiles;


public:
	TileGrid(std::vector<Vertex> vs, std::vector<unsigned int> is, ShaderProgram* prog, std::vector<Tile*> totaltiles, int ident);

	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye);

	std::vector<Tile*> getTiles();
	
	void addTile(Tile* tile);

	Tile* getTile(int index);

	virtual ~TileGrid();
};

