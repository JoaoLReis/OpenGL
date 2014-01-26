#pragma once

#include "Includes.h"

#include "Manager.h"

class Game
{
private:
	bool Running;
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event Event;
	Manager* manager;
public:
	Game();

	int OnExecute();
	bool OnInit();
	bool OnGlewInit();
	bool OnGameInit();
	bool LoadContent();
	void OnEvent(SDL_Event* Event);
	void OnLoop();
	void OnRender();
	void Cleanup();
};