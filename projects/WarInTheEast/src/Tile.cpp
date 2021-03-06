#include "Tile.h"
#include "Piece.h"

Tile::Tile(glm::vec3 position)
{
	adj = new std::vector<Tile*>;
	type = DEFAULT;
	oldtype = DEFAULT;
	pos = glm::vec3(position.x + TILESIZE/2.0f, position.y + TILESIZE/2.0f, position.z);
	gameObject = NULL;
	rank = 0;
	heuristic = 0;
	f_heuristic = 0;
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

int Tile::getRank()
{
	return rank;
}

int Tile::getObjectID()
{
	return gameObject->getID();
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

bool Tile::isObjective()
{
	if (type == EXIT)
		return true;
	else return false;
}

float Tile::getHeuristic()
{
	return heuristic;
}

float Tile::getf_Heuristic()
{
	return f_heuristic;
}

std::string Tile::getPosInString()
{
	std::ostringstream oss;
	oss << pos.x << pos.y << pos.z;
	return oss.str();
}

void Tile::setHeuristic(float v)
{
	heuristic = v;
}

void Tile::setf_Heuristic(float v)
{
	f_heuristic = v;
}

void Tile::addAdj(Tile* tile)
{
	adj->push_back(tile);
}

void Tile::addObj(PieceInstance* piece)
{
	rank = STARTER;
	gameObject = piece;
}

void Tile::removeObj()
{
	gameObject = NULL;
	rank = 0;
}

void Tile::upgradePiece(PieceInstance* piece)
{
	if (rank != ELITE){
		gameObject = piece;
		rank++;
	}
	else
		std::cout << "Tower fully upgraded" << std::endl;

}

Tile::~Tile()
{
}
