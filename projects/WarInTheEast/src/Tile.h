#pragma once
#include "Piece.h"

enum type { BUILD, MOVEMENT, DEFAULT };

class Tile : public Piece
{
	std::vector<Tile*>* adj;
	glm::vec3 pos;
	//glm::vec2 id;
	int type, oldtype;


public:
	Tile();
	Tile(std::vector<Vertex> vs, std::vector<unsigned int> is, ShaderProgram* prog, glm::vec3 position);

	std::vector<Tile*>* getAdj();
	glm::vec3 getPos();
	int getType();
	int getOldType();	
	int getId();

	void setType(int newtype);
	void addAdj(Tile* tile);

	virtual ~Tile();
};

