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
	SDL_GL_SwapWindow(window);
}

void Game::OnEvent(SDL_Event* Event)
{
	int x = 0, y = 0;

	if(Event->type == SDL_QUIT)
	{
		Running = false;
	}
	else
	if (Event->type == SDL_MOUSEMOTION)
	{
		//Get the mouse coordinates 
		x = Event->motion.x;
		y = Event->motion.y;
		std::cout << "Mouse->X : " << x << std::endl;
		std::cout << "Mouse->Y : " << y << std::endl;
	}
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

	int start, end;
	//primary run loop
	while(Running)
	{
		start = SDL_GetTicks();
		while(SDL_PollEvent(&Event))
		{
			OnEvent(&Event);
		}

		OnLoop();
		OnRender();

		end = SDL_GetTicks() - start;
		if(end < 1000 / FRAMES_PER_SECOND)
		{
			SDL_Delay(1000 / FRAMES_PER_SECOND - end);
		}
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