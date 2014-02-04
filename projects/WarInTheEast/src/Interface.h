#include "Includes.h"
#include "UiElement.h"
#include "Piece.h"

class Interface 
{
private:
	std::vector<Vertex> vertexes;
	std::vector<unsigned int> indices;
	std::vector<UiElement> ui;

	ShaderProgram* shaderProg;

	GLuint VboId[2], VaoId, indexBuffer, progID;

public:

	Interface(std::vector<Vertex> vs,
		std::vector<unsigned int> is,
		std::vector<UiElement> ui,
		ShaderProgram* prog);
	
	void createBufferObject();
	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye);
	int checkClick(glm::vec2 mouse);
	
	UiElement getUi(int index);
};