#include "GameData.h"

void GameData::GameDataInit()
{
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