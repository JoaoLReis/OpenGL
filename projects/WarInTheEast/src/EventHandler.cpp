#include "EventHandler.h"

EventHandler::EventHandler(Manager* v)
{
	manager = v;
	DetectCameraMovement = false;
	quitEvent = false;
}

bool EventHandler::handle(SDL_Event* Event)
{
	static float x = 0.0f, y = 0.0f;
	if (Event->type == SDL_WINDOWEVENT_RESIZED)
	{
		glViewport(0, 0, Event->window.data1, Event->window.data2);
	}
	else
	if (Event->type == SDL_QUIT)
	{
		quitEvent = true;
	}
	else
	if (Event->type == SDL_MOUSEWHEEL)
	{
		manager->updateCameraZoom(Event->wheel.y);
	}
	else
	if (Event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (Event->button.button == SDL_BUTTON_MIDDLE)
		{
			if (DetectCameraMovement == true)
			{
				DetectCameraMovement = false;
				/*SDL_SetRelativeMouseMode(SDL_bool(false));
				return true;*/
			}
			else
			{
				DetectCameraMovement = true;
				/*SDL_SetRelativeMouseMode(SDL_bool(true));
				return true;*/
			}
		}
		else
		if (Event->button.button == SDL_BUTTON_LEFT)
		{
			DetectCameraMovement = false;
			manager->tilesRayPick(Event->motion.x, Event->motion.y);
			return true;
		}
		else
		{
			DetectCameraMovement = false;
			/*SDL_SetRelativeMouseMode(SDL_bool(false));
			return true;*/
		}
	}
	else 
	if (Event->type == SDL_MOUSEMOTION)
	{
		//Update mouse coordinates 
		x = Event->motion.x - WINDOW_WIDTH / 2.0f;
		y = -(Event->motion.y - WINDOW_HEIGHT / 2.0f);
		if (DetectCameraMovement)
		{
			manager->updateCameraPosition(x, y);
			manager->updateLastMXY(x, y);
			return true;
		}
		manager->updateLastMXY(x, y);
	}

	return false;
}

bool EventHandler::hasQuitEvent()
{
	return quitEvent;
}