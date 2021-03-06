#pragma once
#include "PieceInstance.h"

enum type { BUILD, MOVEMENT, DEFAULT, ENTRY, EXIT, SPAWN, VISITED };
enum towerRank { STARTER, ADVANCED, ELITE};

class Tile
{
	std::vector<Tile*>* adj;
	glm::vec3 pos;
	
	int type, oldtype, rank;
	float heuristic, f_heuristic;

	PieceInstance* gameObject;

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
	float getHeuristic();
	float getf_Heuristic();
	bool isObjective();
	bool hasObject();

	void setHeuristic(float v);
	void setf_Heuristic(float v);
	std::string getPosInString();
	void setType(int newtype);
	void addAdj(Tile* tile);
	void addObj(PieceInstance* piece);
	void removeObj();

	void upgradePiece(PieceInstance* piece);

	virtual ~Tile();
};

