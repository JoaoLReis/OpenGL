#include "Face.h"

Face::Face(std::vector<GLubyte> *ind)
{
	indexes = ind;
}

void Face::draw()
{
	glDrawElements(GL_TRIANGLES, indexes->size(), GL_UNSIGNED_BYTE, &indexes);
}