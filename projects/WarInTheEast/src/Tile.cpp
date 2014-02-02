#include "Tile.h"
#include "Piece.h"

Tile::Tile(std::vector<Vertex> vs, std::vector<unsigned int> is, ShaderProgram* prog, glm::vec3 position, int ident)
	: Piece(vs, is, prog, ident)
{
	adj = new std::vector<Tile*>;
	type = DEFAULT;
	oldtype = DEFAULT;
	pos = position;
	selected = false;
}

std::vector<Tile*>* Tile::getAdj()
{ 
	return adj; 
}

glm::vec3 Tile::getPos()
{ 
	return pos; 
}

int Tile::getType()
{ 
	return type; 
}

int Tile::getOldType()
{	
	return oldtype; 
}

void Tile::setType(int newtype)
{
	oldtype = type;
	type = newtype;
}

void Tile::addAdj(Tile* tile)
{
	adj->push_back(tile);
}

void Tile::setSelected()
{
	if (selected == false)
		selected = true;
	else selected = false;
}

bool Tile::isSelected()
{
	return selected;
}

Tile::~Tile()
{
}
