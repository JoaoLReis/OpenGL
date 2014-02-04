#include "Tile.h"
#include "Piece.h"

Tile::Tile(glm::vec3 position)
{
	adj = new std::vector<Tile*>;
	type = DEFAULT;
	oldtype = DEFAULT;
	pos = glm::vec3(position.x + TILESIZE/2.0f, position.y + TILESIZE/2.0f, position.z);
	gameObject = NULL;
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

bool Tile::hasObject()
{
	if (gameObject == NULL)
		return false;
	else return true;
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

void Tile::addObj(Piece* piece)
{
	gameObject = piece;
}

void Tile::removeObj()
{
	gameObject = NULL;
}

Tile::~Tile()
{
}
