#include "Includes.h"
#include "Manager.h"

class EventHandler
{
private:
	Manager* manager;
	bool DetectCameraMovement, quitEvent;
public:
	EventHandler(Manager* manager);

	bool handle(SDL_Event* Event);
	bool hasQuitEvent();
	
};