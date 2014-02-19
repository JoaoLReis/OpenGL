#include "GameData.h"

void GameData::GameDataInit()
{
	path = new PathHolder(*new std::vector<Tile*>);
	frame = 0;
}

void GameData::updateFrame(int v)
{
	frame = v;
}

int GameData::getFrame()
{
	return frame;
}