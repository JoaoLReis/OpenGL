#include "Manager.h"

Manager::Manager()
{
	camera = new Camera();
	camera->setPerspective(tFOVY, WINDOW_WIDTH / WINDOW_HEIGHT, tNEAR, tFAR); 
	camera->createViewMatrix(0, 0, -20, 0, 0, 0);
	mapList = new std::vector<Scene*>;
	initMapList();
}

void Manager::draw()
{
	activeScene->draw(camera->getViewMatrix(), camera->getProjectionMatrix(), camera->computeCameraCenter());
}

ShaderProgram* Manager::createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	ShaderProgram *shProg = new ShaderProgram();

	shProg->addVertexShader(vertexShaderPath.c_str());
	shProg->addFragmentShader(fragmentShaderPath.c_str());

	glBindAttribLocation(shProg->getProgram(), VERTICES, "in_Position");
	glBindAttribLocation(shProg->getProgram(), COLORS, "in_Color");
	glBindAttribLocation(shProg->getProgram(), NORMALS, "in_Normal");
	glBindAttribLocation(shProg->getProgram(), TEXTURE, "in_Texture");
	shProg->link();

	return shProg;
}

void Manager::initMapList()
{ 
	activeScene = initMap1(); 
	/*...etc...**/
}

Scene *Manager::initMap1()
{
	std::vector<Piece*> *ps = new std::vector<Piece*>;
	std::vector<Vertex> *vs = new std::vector<Vertex>;
	std::vector<unsigned int> *is = new std::vector<unsigned int>;
	ShaderProgram *shProg = createShaderProgram("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");

	Vertex *v = new Vertex();
	v->XYZW = glm::vec4( -0.5f, -0.5f, 0.0, 1.0f ), v->RGBA = glm::vec4(  0.9f, 0.0f, 0.0f, 1.0f ), v->NORMAL = glm::vec4( 0.0f, 0.0, 1.0f, 1.0f), v->UV = glm::vec2 (0.0f, 0.0f); // 0 - FRONT
	vs->push_back(*v);
	v->XYZW = glm::vec4( 0.5f, -0.5f, 0.0, 1.0f ), v->RGBA = glm::vec4(  0.9f, 0.0f, 0.0f, 1.0f ), v->NORMAL = glm::vec4( 0.0f, 0.0, 1.0f, 1.0f), v->UV = glm::vec2 (1.0f, 0.0f);  // 1
	vs->push_back(*v);
	v->XYZW = glm::vec4( 0.5f, 0.5f, 0.0, 1.0f ), v->RGBA = glm::vec4(  0.9f, 0.0f, 0.0f, 1.0f ), v->NORMAL = glm::vec4( 0.0f, 0.0, 1.0f, 1.0f), v->UV = glm::vec2 (1.0f, 1.0f);  // 2
	vs->push_back(*v);
	v->XYZW = glm::vec4( -0.5f, 0.5f, 0.0, 1.0f ), v->RGBA = glm::vec4(  0.9f, 0.0f, 0.0f, 1.0f ), v->NORMAL = glm::vec4( 0.0f, 0.0, 1.0f, 1.0f), v->UV = glm::vec2 (0.0f, 1.0f);  // 3
	vs->push_back(*v);

	is->push_back(0);
	is->push_back(1);
	is->push_back(2);
	is->push_back(2);
	is->push_back(3);
	is->push_back(0);

	Piece *p = new Piece(*vs, *is, shProg);
	ps->push_back(p);
	return(new Scene(ps));
}