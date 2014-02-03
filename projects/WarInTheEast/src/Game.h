#pragma once

#include "Includes.h"
#include "EventHandler.h"
#include <time.h> 

class Game
{
private:
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event Event;

	Manager* manager;
	EventHandler* eventHandler;

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

	bool isOpenGLError();
	void checkOpenGLError(std::string error);

	void Cleanup();
};