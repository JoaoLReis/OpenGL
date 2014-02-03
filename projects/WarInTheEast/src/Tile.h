#pragma once
#include "Piece.h"

enum type { BUILD, MOVEMENT, DEFAULT };

class Tile
{
	std::vector<Tile*>* adj;
	glm::vec3 pos;
	
	int type, oldtype;
	bool selected;

public:
	Tile();
	Tile(glm::vec3 position);

	std::vector<Tile*>* getAdj();
	glm::vec3 getPos();
	int getType();
	int getOldType();	
	int getId();
	void setSelected();
	bool isSelected();

	void setType(int newtype);
	void addAdj(Tile* tile);

	virtual ~Tile();
};

