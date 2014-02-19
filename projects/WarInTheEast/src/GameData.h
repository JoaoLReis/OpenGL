#pragma once
#include "Includes.h"
#include "PathHolder.h"

class GameData
{
private:
	GameData() {};
	GameData(GameData const&);
	int frame;
	PathHolder *path;

public:
	static GameData& getInstance()
	{
		static	GameData	instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return	instance;
	}
	void GameDataInit();
	
	PathHolder* getPath();
	int getFrame();
	void updateFrame(int v);


};