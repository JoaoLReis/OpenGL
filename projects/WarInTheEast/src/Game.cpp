#include "Game.h"

Game::Game()
{
	window = NULL;
}

bool  Game::isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		std::cerr << "OpenGL ERROR [" << errCode << "]." << std::endl;
	}
	return isError;
}

void Game::checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

//moving updates, math etc
void Game::OnLoop()
{

}

void Game::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	manager->draw();
	SDL_GL_SwapWindow(window);
}

bool Game::OnEvent(SDL_Event* Event)
{
	return eventHandler->handle(Event);
}

bool Game::OnInit()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}

	if (OnOpenglInit() == false)
	{
		return false;
	}

	//creates a window
	if ((window = SDL_CreateWindow("SDL Render Clear", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)) == NULL)
	{
		return false;
	}

	//Creates a SDL opengl context
	context = SDL_GL_CreateContext(window);

	/* Configuration of the Open GL renderer */
	/*Enable back face culling*/
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	/*--------------------*/

	/* Depth buffer setup */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	/*--------------------*/

	/* Stencil buffer setup */
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClearStencil(0.0);
	/*--------------------*/
	
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

bool Game::OnOpenglInit()
{
	/* Request opengl 3.3 context.*/
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	/* Turn on double buffering with a 24bit Z buffer.
	* Note 16 bits or 24 is dependent on system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	/* Sets stencil buffer to 8 bit */
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	return true;
}

bool Game::OnGameInit()
{
	manager = new Manager();
	eventHandler = new EventHandler(manager);
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

	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;

	int start, end;
	//primary run loop
	while(!eventHandler->hasQuitEvent())
	{
		start = SDL_GetTicks();

		OnLoop();
		OnRender();

		while (SDL_PollEvent(&Event))
		{
			if (OnEvent(&Event))
				break;
		}

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