#include "UiElement.h"

UiElement::UiElement(std::vector<Vertex> vs, int t)
{
	bounds = vs;
	tpe = t;
}

int UiElement::testIntersection(glm::vec2 mouse)
{
	if (mouse.y <= bounds[2].XYZW.y &&
		mouse.y >= bounds[0].XYZW.y &&
		mouse.x >= bounds[0].XYZW.x &&
		mouse.x <= bounds[1].XYZW.x)
	{
		std::cout << "MOUSE in UIElem - " << tpe << " - at-> " << mouse.x << " " << mouse.y << std::endl;
		return tpe;
	}
	else return -1;
}