#pragma once
#include "Piece.h"
#include "Tile.h"

class TileGrid : public Piece
{
	std::vector<Tile*> tiles;
	int selected;

public:
	TileGrid(std::vector<Vertex> vs, std::vector<unsigned int> is, ShaderProgram* prog, std::vector<Tile*> totaltiles, int ident);

	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye);
	bool checkClick(glm::vec4 pos, glm::vec3 direction);
	void addTile(Tile* tile);
	void setSelected(int index);

	std::vector<Tile*> getTiles();
	Tile* getTile(int index);
	int whichSelected();

	virtual ~TileGrid();
};

