#include "Includes.h"
#include "Textures.h"
#include "Vertex.h"

class UiElement
{
	std::vector<Vertex> bounds;
	Texture* tex;
	int tpe;
public:
	UiElement(std::vector<Vertex> bounds, Texture* tex, int i);
	
	int testIntersection(glm::vec2 mouse);

	void draw(int i, GLuint progID);
}; 