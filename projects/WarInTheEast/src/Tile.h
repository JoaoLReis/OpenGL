#pragma once
#include "Piece.h"

enum type { BUILD, MOVEMENT, DEFAULT };
enum towerRank { STARTER, ADVANCED, ELITE};

class Tile
{
	std::vector<Tile*>* adj;
	glm::vec3 pos;
	
	int type, oldtype, rank;

	Piece* gameObject;


public:
	Tile();
	Tile(glm::vec3 position);

	std::vector<Tile*>* getAdj();
	glm::vec3 getPos();
	int getType();
	int getOldType();	
	int getId();
	int getRank();
	int getObjectID();
	bool hasObject();

	void setType(int newtype);
	void addAdj(Tile* tile);
	void addObj(Piece* piece);
	void removeObj();

	void upgradePiece(Piece* piece);

	virtual ~Tile();
};

