#include "Manager.h"

Manager::Manager()
{
	camera = new Camera();
	camera->setPerspective(tFOVY, WINDOW_WIDTH / WINDOW_HEIGHT, tNEAR, tFAR);
	camera->setCenter(glm::vec3(-NUMTILESX / 2, -NUMTILESY / 2, -40.0f));
	camera->updateCamera();
	mapList = new std::vector<Scene*>;

	initMapList();
	initInterface();
}

void Manager::tilesRayPick(float x, float y)
{
	glm::vec2 mouse;

	mouse.x = x- WINDOW_WIDTH / 2;
	mouse.y = -(y - WINDOW_HEIGHT / 2);

	mouse.x /= (WINDOW_WIDTH / 2);
	mouse.y /= (WINDOW_HEIGHT / 2);

	glm::mat4 toWorld = glm::inverse(camera->getProjectionMatrix() * camera->getViewMatrix());

	glm::vec4 from = toWorld * glm::vec4(mouse, -1.0f, 1.0f);
	glm::vec4 to = toWorld * glm::vec4(mouse, 1.0f, 1.0f);

	from /= from.w; //perspective divide ("normalize" homogeneous coordinates)
	to /= to.w;

	glm::vec3 direction = glm::vec3(to - from);

	/*First Intersect with UI elements.
	then check with the tileGrid*/
	/*-------------------------------*/
	int uiElem = interface->checkClick(mouse);
	if (uiElem == -1)
		activeScene->getTileGrid()->checkClick(from, direction);
	else if (uiElem == BUILD)
	{
		if (activeScene->getTileGrid()->whichSelected() != -1)
			activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected())->setType(BUILD);
	}
	else if (uiElem == MOVEMENT)
	{
		if (activeScene->getTileGrid()->whichSelected() != -1)
			activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected())->setType(MOVEMENT);
	}
	else if (uiElem == DEFAULT)
	{
		if (activeScene->getTileGrid()->whichSelected() != -1)
			activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected())->setType(DEFAULT);
	}
	/*-------------------------------*/
}

void Manager::updateCameraRotation(float x, float y)
{
	camera->setRotationAngleY(camera->getRotationAngleY() + (x - camera->getLast_mx()));
	camera->setRotationAngleX(camera->getRotationAngleX() + (y - camera->getLast_my()));
	camera->setLast_mx(x);
	camera->setLast_my(y);
	camera->updateCamera();
}

void Manager::updateCameraPosition(float x, float y)
{
	updateCameraRotation(x, y);
	//camera->setCenter(glm::vec3(camera->getCenter().x + ((x - camera->getLast_mx()) / -camera->getCenter().z), camera->getCenter().y + ((y - camera->getLast_my()) / -camera->getCenter().z), camera->getCenter().z));
}

void Manager::updateLastMXY(float x, float y)
{
	camera->setLast_mx(x);
	camera->setLast_my(y);
	camera->updateCamera();
}

void Manager::updateCameraZoom(int amount)
{
	if(amount > 0)
		camera->setCenter(glm::vec3(camera->getCenter().x, camera->getCenter().y, camera->getCenter().z + 1));
	else camera->setCenter(glm::vec3(camera->getCenter().x, camera->getCenter().y, camera->getCenter().z - 1));
}

void Manager::draw()
{
	activeScene->draw(camera->getViewMatrix(), camera->getProjectionMatrix(), camera->computeCameraCenter());
	interface->draw(glm::mat4(1.0), glm::mat4(1.0), camera->computeCameraCenter());
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
	Piece* p;
	PieceReader::getInstance().init();
	std::vector<Piece*> *ps = new std::vector<Piece*>;
	std::vector<Vertex> *vs = new std::vector<Vertex>;
	std::vector<unsigned int> *is = new std::vector<unsigned int>;
	Scene* scene = new Scene(ps);
	ShaderProgram *shProg = createShaderProgram("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");

	/*
	+++++++++++++++++++++++++++++++++++++++++++ PREFAB ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	*/
	/** /
	Vertex v = *(new Vertex());
	float size = 0.5;

	v.XYZW = glm::vec4(size, size, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, 1.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 2
	vs->push_back(v);
	v.XYZW = glm::vec4(size, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, 1.0f, 1.0f), v.UV = glm::vec2(1.0f, 1.0f);  // 1
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, 1.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f); // 0 - FRONT
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, 1.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 0
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, size, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, 1.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(v);
	v.XYZW = glm::vec4(size, size, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, 1.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 2
	vs->push_back(v);

	v.XYZW = glm::vec4(size, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 6
	vs->push_back(v);
	v.XYZW = glm::vec4(size, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 1.0f);  // 5
	vs->push_back(v);
	v.XYZW = glm::vec4(size, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 1 - RIGHT
	vs->push_back(v);
	v.XYZW = glm::vec4(size, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 1
	vs->push_back(v);
	v.XYZW = glm::vec4(size, size, size, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 2
	vs->push_back(v);
	v.XYZW = glm::vec4(size, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 6
	vs->push_back(v);

	v.XYZW = glm::vec4(0.0f, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 7
	vs->push_back(v);
	v.XYZW = glm::vec4(size, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 1.0f);  // 6
	vs->push_back(v);
	v.XYZW = glm::vec4(size, size, size, 1.0f), v.RGBA = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 2 - TOP
	vs->push_back(v);
	v.XYZW = glm::vec4(size, size, size, 1.0f), v.RGBA = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 2
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, size, size, 1.0f), v.RGBA = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 7
	vs->push_back(v);

	v.XYZW = glm::vec4(0.0f, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, -1.0, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 7
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, -1.0, 1.0f), v.UV = glm::vec2(1.0f, 1.0f);  // 4
	vs->push_back(v);
	v.XYZW = glm::vec4(size, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, -1.0, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 5 - BACK
	vs->push_back(v);
	v.XYZW = glm::vec4(size, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, -1.0, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 5
	vs->push_back(v);
	v.XYZW = glm::vec4(size, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, -1.0, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 6
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.0f, 0.9f, 0.9f, 1.0f), v.NORMAL = glm::vec4(0.0f, 0.0, -1.0, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 7
	vs->push_back(v);


	v.XYZW = glm::vec4(0.0f, size, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(-1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(-1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 1.0f);  // 0
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(-1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 4 - LEFT
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(-1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 4
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, size, 0.0f, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(-1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 7
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, size, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.0f, 0.9f, 1.0f), v.NORMAL = glm::vec4(-1.0f, 0.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(v);

	v.XYZW = glm::vec4(size, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.9f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, -1.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 5
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.9f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, -1.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 1.0f);  // 4
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, -1.0, 0.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 0 - BOTTOM
	vs->push_back(v);
	v.XYZW = glm::vec4(0.0f, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, -1.0, 0.0f, 1.0f), v.UV = glm::vec2(0.0f, 1.0f);  // 0
	vs->push_back(v);
	v.XYZW = glm::vec4(size, 0.0f, size, 1.0f), v.RGBA = glm::vec4(0.9f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, -1.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 1
	vs->push_back(v);
	v.XYZW = glm::vec4(size, 0.0f, 0.0f, 1.0f), v.RGBA = glm::vec4(0.9f, 0.9f, 0.0f, 1.0f), v.NORMAL = glm::vec4(0.0f, -1.0, 0.0f, 1.0f), v.UV = glm::vec2(1.0f, 0.0f);  // 5
	vs->push_back(v);

	for (int i = 0; i <= 35; i++)
	{
		is->push_back(i);
	}

	Piece *p = new Piece(*vs, *is, shProg);
	ps->push_back(p);

	vs->clear();
	is->clear();

	/**/

/*
+++++++++++++++++++++++++++++++++++++++++++++++  GRID  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

	Vertex vert = *(new Vertex());
	float size = TILESIZE;
	std::vector<Tile*> *totaltiles = new std::vector<Tile*>;
	std::vector<Vertex> *tilevertexes = new std::vector<Vertex>;
	std::vector<unsigned int> *tileindexes = new std::vector<unsigned int>;
	Tile * t;
	for (int i = 0; i < NUMTILESY; i++){
		for (int k = 0; k < NUMTILESX; k++){
			vert.XYZW = glm::vec4(k, i, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 2
			vs->push_back(vert);
			tilevertexes->push_back(vert);
			vert.XYZW = glm::vec4(k + size, i, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
			vs->push_back(vert);
			tilevertexes->push_back(vert);
			vert.XYZW = glm::vec4(k + size, i + size, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 1.0f); // 0 - FRONT
			vs->push_back(vert);
			tilevertexes->push_back(vert);
			vert.XYZW = glm::vec4(k, i + size, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f);  // 1
			vs->push_back(vert);
			tilevertexes->push_back(vert);

			for (int j = 0; j < 4; j++)
			{
				is->push_back(j);
			}

			t = new Tile(glm::vec3(k, i, 0));

			totaltiles->push_back(t);

			vs->clear();
			is->clear();
		}
	}

	for (int l = 0; l < tilevertexes->size(); l++)
	{
		tileindexes->push_back(l);
	}

	Piece* tgrid = new TileGrid(*tilevertexes, *tileindexes, shProg, *totaltiles, scene->getId());
	ps->push_back(tgrid);

	totaltiles->clear();
	tileindexes->clear();
	tilevertexes->clear();

	/*
	+++++++++++++++++++++++++++++++++++++++++++++++ CYLINDER +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	*/
	/** /
	PieceReader::getInstance().readObject("..\\objects\\lightNormalTower.obj");
	
	*vs = PieceReader::getInstance().getVertices();
	*is = PieceReader::getInstance().getIndices();
	p = new Piece(*vs, *is, shProg, scene->getId()); 
	ps->push_back(p);

	PieceReader::getInstance().clearAll();
	vs->clear();
	is->clear();
	
	/**/
	/*
	+++++++++++++++++++++++++++++++++++++++++++++++           +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	*/

	return(scene);
}

void Manager::initInterface()
{
	ShaderProgram *sh = createShaderProgram("..\\shaders\\vertex_shader_2d.glsl", "..\\shaders\\fragment_shader_2d.glsl");
	Vertex vert = *(new Vertex());
	std::vector<Vertex> *vs = new std::vector<Vertex>;
	std::vector<Vertex> *tmp = new std::vector<Vertex>;
	std::vector<unsigned int> *is = new std::vector<unsigned int>;
	std::vector<UiElement> *ui = new std::vector<UiElement>;
	Texture* tex;

	vert.XYZW = glm::vec4(-1.0, -0.6, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 0.0f);  // 2
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.6, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.4, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, -1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, -0.4, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, -1.0f);  // 1
	vs->push_back(vert);
	tmp->push_back(vert);
	tex = new Texture2D();
	tex->load("..\\textures\\Build_Pick.psd");
	ui->push_back(*new UiElement(*tmp, tex, BUILD));
	tmp->clear(); 

	vert.XYZW = glm::vec4(-1.0, -0.8, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 0.0f);  // 2
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.8, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.6, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, -1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, -0.6, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, -1.0f);  // 1
	vs->push_back(vert);
	tmp->push_back(vert);
	tex = new Texture2D();
	tex->load("..\\textures\\Movement_Pick.psd");
	ui->push_back(*new UiElement(*tmp, tex, MOVEMENT));
	tmp->clear();

	vert.XYZW = glm::vec4(-1.0, -1.0, 0.0f, 1.0f), vert.RGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 0.0f);  // 2
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -1.0, 0.0f, 1.0f), vert.RGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.8, 0.0f, 1.0f), vert.RGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, -1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, -0.8, 0.0f, 1.0f), vert.RGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, -1.0f);  // 1
	vs->push_back(vert);
	tmp->push_back(vert);
	tex = new Texture2D();
	tex->load("..\\textures\\Default_Pick.psd");
	ui->push_back(*new UiElement(*tmp, tex, DEFAULT));
	tmp->clear();

	for (int j = 0; j < vs->size(); j++)
	{
		is->push_back(j);
	}
	
	interface = new Interface(*vs, *is, *ui, sh);
}

Scene* Manager::getScene()
{
	return activeScene;
}

void Manager::saveTileGrid()
{
	std::string savename = "TileGrid";
	std::string pos = ".tlg";

	savename.append(pos);

	std::ofstream out(savename);
	std::ofstream file;

	file.open(savename);

	file << "Dim " << NUMTILESX << ", " << NUMTILESY << std::endl;
	file << std::endl;

	TileGrid* tgrid = activeScene->getTileGrid(); 
	std::vector<Tile*> tiles = tgrid->getTiles();

	for (auto it = tiles.begin(); it != tiles.end(); ++it)
	{
		file << "Ot " << (*it)->getOldType() << std::endl;
		file << "T " << (*it)->getType() << std::endl;
		file << std::endl;
	}

	file << "end";
	file.close();
}

void Manager::loadTileGrid()
{
	std::ifstream file;
	file.open("TileGrid.tlg");

	std::string lineID = std::string();
	char *line = new char();
	TileGrid* tgrid = activeScene->getTileGrid();
	int dimx, dimy, oldType, type, index = 0;
	
	if (file.peek() == 'Dim'){
		file.get(); // to consume the peeked character
		file >> dimx >> dimy;
		file.get(); // to consume the \n
	}

	while (file.getline(line, 256))
	{
		if (strcmp(line, "end") != 0){
			file >> lineID;
			if (lineID.compare("Ot") == 0){
				file >> oldType;
				file.get();
				tgrid->getTile(index)->setType(oldType);
			}
			file >> lineID;
			if (lineID.compare("T") == 0){
				file >> type;
				file.get();
				tgrid->getTile(index++)->setType(type);
			}
		}
		else
			break;
	}

	file.close();
}

void Manager::addPieceToTile(int index, int type)
{
	Tile* tile = getScene()->getTileGrid()->getTile(getScene()->getTileGrid()->whichSelected());
	Piece* p;
	Texture* tex = new Texture2D();
	switch (type)
	{

	case NORMAL:
		PieceReader::getInstance().readObject("..\\objects\\lightNormalTower.obj");
		tex->load("..\\textures\\Tower_Normal.psd");
		p = new Piece(PieceReader::getInstance().getVertices(), PieceReader::getInstance().getIndices(), createShaderProgram("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl"), tex, activeScene->getId());
		p->translate(tile->getPos());
		tile->addObj(p);
		activeScene->addPiece(p);
		break;
	case HEAVY:
		break;
	case SLOW:
		break;
	default:
		break;
	}

}
