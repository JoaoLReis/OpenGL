#include "Includes.h"
#include "Vertex.h"

class Face
{
private:

	std::vector<GLubyte> *indexes;

public:

	Face(std::vector<GLubyte> *indexes);

	void draw();

	void addIndex(GLubyte v) 
	{
		indexes->push_back(v);
	}
};