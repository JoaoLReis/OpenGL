#include "Interface.h"

Interface::Interface (std::vector<Vertex> vs, 
					  std::vector<unsigned int> is,
					  std::vector<UiElement> elems,
					  ShaderProgram* prog) : Drawable()
{
	vertexes = vs;
	indices = is;
	shaderProg = prog;
	ui = elems;
	createBufferObject();
}

void Interface::createBufferObject()
{
	progID = shaderProg->getProgram();

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(2, VboId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexes.size(), &vertexes[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(vertexes[0].XYZW));

	glEnableVertexAttribArray(NORMALS);
	glVertexAttribPointer(NORMALS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(vertexes[0].XYZW) + sizeof(vertexes[0].RGBA)));

	glEnableVertexAttribArray(TEXTURE);
	glVertexAttribPointer(TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(vertexes[0].XYZW) + sizeof(vertexes[0].RGBA) + sizeof(vertexes[0].NORMAL)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Interface::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye)
{

	glBindVertexArray(VaoId);
	glUseProgram(progID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	int i = 0;
	for (std::vector<UiElement>::iterator it = ui.begin(); it != ui.end(); ++it)
	{
		(*it).draw(i, progID); 
		i += 4;
	}
	glUseProgram(0);
	glBindVertexArray(0);
}

int Interface::checkClick(glm::vec2 mouse)
{
	int i = 0;
	for (std::vector<UiElement>::iterator it = ui.begin(); it != ui.end(); ++it)
	{
		i = (*it).testIntersection(mouse);
		if (i != -1)
		{
			return i;
		}
	}
	return -1;
}

UiElement Interface::getUi(int index)
{
	return ui[index];
}