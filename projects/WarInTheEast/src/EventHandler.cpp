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
		return true;
	}
	else
	if (Event->type == SDL_QUIT)
	{
		quitEvent = true;
		return true;
	}
	else
	if (Event->type == SDL_MOUSEWHEEL)
	{
		manager->updateCameraZoom(Event->wheel.y);
		return true;
	}
	else
	if (Event->type == SDL_KEYDOWN)
	{
		if (Event->key.keysym.sym == SDLK_s){
			manager->saveTileGrid();
		}
		else if (Event->key.keysym.sym == SDLK_l)
		{
			manager->loadTileGrid();
		}
		else if (Event->key.keysym.sym == SDLK_b)
		{
			manager->getScene()->getTileGrid()->getTile(manager->getScene()->getTileGrid()->whichSelected())->setType(BUILD);
		}
		else if (Event->key.keysym.sym == SDLK_n)
		{
			manager->getScene()->getTileGrid()->getTile(manager->getScene()->getTileGrid()->whichSelected())->setType(MOVEMENT);
		}
		else if (Event->key.keysym.sym == SDLK_m)
		{
			manager->getScene()->getTileGrid()->getTile(manager->getScene()->getTileGrid()->whichSelected())->setType(DEFAULT);
		}
		else if (Event->key.keysym.sym == SDLK_t)
		{
			if (manager->getScene()->getTileGrid()->getTile(manager->getScene()->getTileGrid()->whichSelected())->getType() == BUILD)
				manager->addPieceToTile(manager->getScene()->getTileGrid()->whichSelected(), 0);
		}
		else if (Event->key.keysym.sym == SDLK_u)
		{
			if (manager->getScene()->getTileGrid()->getTile(manager->getScene()->getTileGrid()->whichSelected())->hasObject())
				manager->upgradePieceInTile(manager->getScene()->getTileGrid()->whichSelected());
		}
		else if (Event->key.keysym.sym == SDLK_UP)
		{
			int selected = manager->getScene()->getTileGrid()->whichSelected();
			if (selected + NUMTILESX < (NUMTILESX * NUMTILESY))
				manager->getScene()->getTileGrid()->setSelected(selected + NUMTILESX);
			
		}
		else if (Event->key.keysym.sym == SDLK_DOWN)
		{
			int selected = manager->getScene()->getTileGrid()->whichSelected();
			if (selected >= NUMTILESX)
				manager->getScene()->getTileGrid()->setSelected(selected - NUMTILESX);
		}
		else if (Event->key.keysym.sym == SDLK_LEFT)
		{
			int selected = manager->getScene()->getTileGrid()->whichSelected();
			if (selected != 0)
				manager->getScene()->getTileGrid()->setSelected(selected - 1);
		}
		else if (Event->key.keysym.sym == SDLK_RIGHT)
		{
			int selected = manager->getScene()->getTileGrid()->whichSelected();
			if (selected != (NUMTILESX * NUMTILESY - 1))
				manager->getScene()->getTileGrid()->setSelected(selected + 1);
		}
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