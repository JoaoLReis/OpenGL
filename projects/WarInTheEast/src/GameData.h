#pragma once
#include "Includes.h"

class GameData
{
private:
	GameData() {};
	GameData(GameData const&);
	int frame;

public:
	static GameData& getInstance()
	{
		static	GameData	instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return	instance;
	}
	void GameDataInit();
	
	int getFrame();
	void updateFrame(int v);
};