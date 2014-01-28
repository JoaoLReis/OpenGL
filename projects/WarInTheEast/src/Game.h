#pragma once

#include "Includes.h"
#include "Manager.h"
#include <time.h> 

class Game
{
private:
	bool Running;
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event Event;

	Manager* manager;

	int frame;

public:
	Game();

	int OnExecute();
	bool OnInit();
	bool OnGlewInit();
	bool OnGameInit();
	bool OnOpenglInit();
	bool LoadContent();
	bool OnEvent(SDL_Event* Event);
	void OnLoop();
	void OnRender();
	void Cleanup();
};