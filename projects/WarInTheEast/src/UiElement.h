#include "Includes.h"
#include "Vertex.h"

class UiElement
{
	std::vector<Vertex> bounds;
	int tpe;
public:
	UiElement(std::vector<Vertex> bounds, int i);
	
	int testIntersection(glm::vec2 mouse);
}; 