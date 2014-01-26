#include "Game.h"

Game::Game()
{
	window = NULL;
	Running = true;
}

//moving updates, math etc
void Game::OnLoop()
{

}

void Game::OnRender()
{
	manager->draw();
}

void Game::OnEvent(SDL_Event* Event)
{
	if(Event->type == SDL_QUIT)
	{
		Running = false;
	}

	SDL_GL_SwapWindow(window);
}

bool Game::OnInit()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}

	//creates a window
	if((window = SDL_CreateWindow("SDL Render Clear", 100, 100, 640, 480, SDL_WINDOW_OPENGL)) == NULL)
	{
		return false;
	}

	//SDL opengl context
	context = SDL_GL_CreateContext(window);
	return true;
}

bool Game::OnGlewInit()
{
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	printf("%u\n", vertexBuffer);
	return true;
}

bool Game::OnGameInit()
{
	manager = new Manager();
	return true;
}

int Game::OnExecute()
{
	if(OnInit() == false)
	{
		return -1;
	}

	if(OnGlewInit() == false)
	{
		return -1;
	}

	if(OnGameInit() == false)
	{
		return -1;
	}

	//primary run loop
	while(Running)
	{
		while(SDL_PollEvent(&Event))
		{
			OnEvent(&Event);
		}
		OnLoop();
		OnRender();
	}

	Cleanup();

	return 0;
}

void Game::Cleanup()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}