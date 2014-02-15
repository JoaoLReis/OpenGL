#include "Includes.h"
#include "UiElement.h"
#include "Piece.h"
#include "Drawable.h"

class Interface : public Drawable
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
	
	void move(float x, float y, float z) {};
	void rotate() {};

	/*Not implemented*/
	Drawable* clone(){ return 0; };
	int getID(){ return 0; };
	/*---------------*/

	UiElement getUi(int index);
};