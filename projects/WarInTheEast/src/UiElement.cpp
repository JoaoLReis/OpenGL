#include "UiElement.h"

UiElement::UiElement(std::vector<Vertex> vs, Texture* t, int te)
{
	bounds = vs;
	tpe = te;
	tex = t;
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

void UiElement::draw(int i, GLuint progID)
{
	if (tex != 0)
	{
		tex->actBindTexture();
		glUniform1f(glGetUniformLocation(progID, "textured"), true);
		glUniform1i(glGetUniformLocation(progID, "Texture0"), 0);
	}
	else
	{
		glUniform1f(glGetUniformLocation(progID, "textured"), false);
	}
	glDrawElementsBaseVertex(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)0, i);
}