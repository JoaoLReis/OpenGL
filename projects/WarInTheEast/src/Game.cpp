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
	glClear(GL_COLOR_BUFFER_BIT);
	manager->draw();
	SDL_GL_SwapWindow(window);
}

bool Game::OnEvent(SDL_Event* Event)
{

	static int x = 0, y = 0;
	if(Event->type == SDL_QUIT)
	{
		Running = false;
	}
	else
	if (Event->type == SDL_MOUSEMOTION)
	{
		//Get the mouse coordinates 
		x = Event->motion.x - WINDOW_WIDTH / 2;
		y = -(Event->motion.y - WINDOW_HEIGHT / 2);

		std::cout << "Mouse->X : " << x << std::endl;
		std::cout << "Mouse->Y : " << y << std::endl;
		manager->updateCameraRotation(x, y);	
		return true;
	}
	return false;
}

bool Game::OnInit()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}
	
	//creates a window
	if ((window = SDL_CreateWindow("SDL Render Clear", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL)
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
	GLenum err = glewInit();
	
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}
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
			if (OnEvent(&Event))
				break;
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