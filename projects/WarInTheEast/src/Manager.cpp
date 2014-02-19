#include "Manager.h"

Manager::Manager()
{
	camera = new Camera();
	camera->setPerspective(tFOVY, WINDOW_WIDTH / WINDOW_HEIGHT, tNEAR, tFAR);
	camera->setCenter(glm::vec3(-NUMTILESX / 2, -NUMTILESY / 2, -40.0f));
	camera->updateCamera();
	mapList = new std::vector<Scene*>;
	preloadedObjs = new std::vector<Piece*>;
	PieceReader::getInstance().init();
	ShaderProgram* sh = createShaderProgram("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");
	ShaderProgram* sh2D = createShaderProgram("..\\shaders\\vertex_shader_2d.glsl", "..\\shaders\\fragment_shader_2d.glsl");
	starter = 0;
	obj = 0;
	initInterface(sh2D);
	initMapList(sh);
	preLoadPieces(sh);
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
	else if (uiElem == SPAWN)
	{
		if (activeScene->getTileGrid()->whichSelected() != -1)
		{
			if (starter != NULL && obj != NULL)
			{
				Spawner* s = new Spawner(starter, obj);
			}
		}
	}
	else if (uiElem == ENTRY)
	{
		if (activeScene->getTileGrid()->whichSelected() != -1) 
		{
			activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected())->setType(ENTRY);
			if (starter != NULL)
			{
				starter->setType(DEFAULT);
				starter = activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected());
			} 
			else starter = activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected());
		}
	}
	else if (uiElem == EXIT)
	{
		if (activeScene->getTileGrid()->whichSelected() != -1) 
		{
			activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected())->setType(EXIT);
			if (obj != NULL)
			{
				obj->setType(DEFAULT);
				obj = activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected());
			}
			else obj = activeScene->getTileGrid()->getTile(activeScene->getTileGrid()->whichSelected());
		}
	}
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

void Manager::initMapList(ShaderProgram* sh)
{ 
	activeScene = initMap1(sh);
	/*...etc...**/
}

void setInstructions()
{
	std::cout << "Hello dear user!!!" << std::endl;
	std::cout << "To play the game use the following keys:" << std::endl;
	std::cout << "b -> change tile to build" << std::endl;
	std::cout << "n -> change tile to movement" << std::endl;
	std::cout << "m -> change tile to default" << std::endl;
	std::cout << "t -> build turret on selected build tile" << std::endl;
	std::cout << "u -> upgrade turret on selected tile" << std::endl;
	std::cout << "r -> remove turret on selected tile" << std::endl;
	std::cout << "s/l -> save/load tile grid" << std::endl;
	std::cout << "arrow keys -> move around the tile grid" << std::endl;
	std::cout << "Thank you and have fun!!!! =D" << std::endl;
}

Scene *Manager::initMap1(ShaderProgram* shProg)
{
	setInstructions();
	//Piece* p;
	std::vector<Drawable*> *ps = new std::vector<Drawable*>;
	std::vector<Vertex> *vs = new std::vector<Vertex>;
	std::vector<unsigned int> *is = new std::vector<unsigned int>;
	Scene* scene = new Scene(ps);

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

	for (unsigned int l = 0; l < tilevertexes->size(); l++)
	{
		tileindexes->push_back(l);
	}

	glm::vec3 pos;
	float index, index0, index1, index2, index3;
	for (std::vector<Tile*>::iterator it = totaltiles->begin(); it != totaltiles->end(); ++it)
	{
		pos = (*it)->getPos();
		index = std::floor(pos.x) + std::floor(pos.y)*NUMTILESX;

		index0 = std::floor(pos.x + TILESIZE) + std::floor(pos.y)*NUMTILESX;
		if (index0 < NUMTILESX * NUMTILESY)
			(*it)->addAdj(totaltiles->at(index0));

		index1 = std::floor(pos.x - TILESIZE) + std::floor(pos.y)*NUMTILESX;
		if (index1 >= 0 )
			(*it)->addAdj(totaltiles->at(index1));

		index2 = std::floor(pos.x) + std::floor(pos.y + TILESIZE)*NUMTILESX;
		if (index2 < NUMTILESX * NUMTILESY)
			(*it)->addAdj(totaltiles->at(index2));

		index3 = std::floor(pos.x) + std::floor(pos.y - TILESIZE)*NUMTILESX;
		if (index3 >= 0 )
			(*it)->addAdj(totaltiles->at(index3));
	}

	Piece* tgrid = new TileGrid(*tilevertexes, *tileindexes, shProg, *totaltiles, scene->getId());
	ps->push_back(tgrid);

	//createEnemy(sh);
	return(scene);
}

void Manager::initInterface(ShaderProgram* sh)
{
	Vertex vert = *(new Vertex());
	std::vector<Vertex> *vs = new std::vector<Vertex>;
	std::vector<Vertex> *tmp = new std::vector<Vertex>;
	std::vector<unsigned int> *is = new std::vector<unsigned int>;
	std::vector<UiElement> *ui = new std::vector<UiElement>;
	Texture* tex;

	vert.XYZW = glm::vec4(-1.0, 0.0, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 0.0f);  // 2
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, 0.0, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, 0.2, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, 0.2, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 1.0f);  // 1
	vs->push_back(vert);
	tmp->push_back(vert);
	tex = new Texture2D();
	tex->load("..\\textures\\Spawn_Pick.psd");
	ui->push_back(*new UiElement(*tmp, tex, SPAWN));
	tmp->clear();

	vert.XYZW = glm::vec4(-1.0, -0.2, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 0.0f);  // 2
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.2, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.0, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, -0.0, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 1.0f);  // 1
	vs->push_back(vert);
	tmp->push_back(vert);
	tex = new Texture2D();
	tex->load("..\\textures\\Entry_Pick.psd");
	ui->push_back(*new UiElement(*tmp, tex, ENTRY));
	tmp->clear();

	vert.XYZW = glm::vec4(-1.0, -0.4, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 0.0f);  // 2
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.4, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.2, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, -0.2, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 1.0f);  // 1
	vs->push_back(vert);
	tmp->push_back(vert);
	tex = new Texture2D();
	tex->load("..\\textures\\Exit_Pick.psd");
	ui->push_back(*new UiElement(*tmp, tex, EXIT));
	tmp->clear();

	vert.XYZW = glm::vec4(-1.0, -0.6, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 0.0f);  // 2
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.6, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-0.8, -0.4, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, -0.4, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 1.0f);  // 1
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
	vert.XYZW = glm::vec4(-0.8, -0.6, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, -0.6, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 1.0f);  // 1
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
	vert.XYZW = glm::vec4(-0.8, -0.8, 0.0f, 1.0f), vert.RGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f); // 0 - FRONT
	vs->push_back(vert);
	tmp->push_back(vert);
	vert.XYZW = glm::vec4(-1.0, -0.8, 0.0f, 1.0f), vert.RGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 1.0f);  // 1
	vs->push_back(vert);
	tmp->push_back(vert);
	tex = new Texture2D();
	tex->load("..\\textures\\Default_Pick.psd");
	ui->push_back(*new UiElement(*tmp, tex, DEFAULT));
	tmp->clear();

	for (unsigned int j = 0; j < vs->size(); j++)
	{
		is->push_back(j);
	}
	
	interface = new Interface(*vs, *is, *ui, sh);
}

void Manager::loadPiece(ShaderProgram* sh, std::string objpath, std::string texpath)
{
	Texture* tex = new Texture2D();
	PieceReader::getInstance().readObject(objpath);
	tex->load(texpath);
	Piece *p = new Piece(PieceReader::getInstance().getVertices(), PieceReader::getInstance().getIndices(), sh, tex, -1);
	preloadedObjs->push_back(p);
	PieceReader::getInstance().clearAll();
	x = new PieceAggregate(p);
	activeScene->addPiece(x);
}

void Manager::preLoadPieces(ShaderProgram* sh)
{
	loadPiece(sh, "..\\objects\\lightNormalTower.obj", "..\\textures\\Tower_Normal.psd");
	loadPiece(sh, "..\\objects\\lightAdvancedTower.obj", "..\\textures\\Tower_Normal.psd");
	loadPiece(sh, "..\\objects\\lightEliteTower.obj", "..\\textures\\Tower_Normal.psd");
	//loadPiece(sh, "..\\objects\\wisp.obj", "..\\textures\\wisp.psd");
	
	/*PieceReader::getInstance().readObject("..\\objects\\lightAdvancedTower.obj");
	tex->load("..\\textures\\Tower_Normal.psd");
	p = new Piece(PieceReader::getInstance().getVertices(), PieceReader::getInstance().getIndices(), sh, tex, -2);
	preloadedObjs->push_back(p);
	PieceReader::getInstance().clearAll();

	PieceReader::getInstance().readObject("..\\objects\\lightEliteTower.obj");
	tex->load("..\\textures\\Tower_Normal.psd");
	p = new Piece(PieceReader::getInstance().getVertices(), PieceReader::getInstance().getIndices(), sh, tex, -2);
	preloadedObjs->push_back(p);
	PieceReader::getInstance().clearAll();*/
}

void Manager::createEnemy(ShaderProgram* sh)
{
	PieceInstance *piece = new Enemy(preloadedObjs->at(WISP-1), preloadedObjs->at(WISP-1)->getOrientation(), preloadedObjs->at(WISP-1)->getTransformation());
	piece->setID(activeScene->getId());
	piece->reset();
	piece->translate(glm::vec3(0.0, 0.0, 0.0));
	//starter->addObj(piece);
	x = (PieceAggregate*)activeScene->getPiece(WISP);
	x->addPiece(piece);
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
	PieceInstance* p;
	Tile* tile = getScene()->getTileGrid()->getTile(index);
	Texture* tex = new Texture2D();
	switch (type)
	{

	case STARTER:
		if (!tile->hasObject())
		{
			x = (PieceAggregate*) activeScene->getPiece(NORMAL_TOWER);
			p = new Tower(preloadedObjs->at(NORMAL_TOWER - 1), preloadedObjs->at(NORMAL_TOWER - 1)->getOrientation(), preloadedObjs->at(NORMAL_TOWER-1)->getTransformation());
			p->setID(activeScene->getId());
			p->reset();
			p->translate(tile->getPos());
			tile->addObj(p);
			x->addPiece(p);
		}
		break;
	case ADVANCED:
		break;
	case ELITE:
		break;
	default:
		break;
	}

}

void Manager::upgradePieceInTile(int index)
{
	Tile* tile = getScene()->getTileGrid()->getTile(index);
	PieceInstance* p;
	Texture* tex = new Texture2D();
	int type = tile->getRank();

	switch (type)
	{
	case STARTER:
		p = new Tower(preloadedObjs->at(NORMAL_TOWER - 1), preloadedObjs->at(NORMAL_TOWER - 1)->getOrientation(), preloadedObjs->at(NORMAL_TOWER-1)->getTransformation());
		p->translate(tile->getPos());
		activeScene->removePieceFromAggregate(NORMAL_TOWER, tile->getObjectID());
		tile->upgradePiece(p);
		activeScene->addPieceToAggregate(ADVANCED_TOWER, p);
		break;

	case ADVANCED:
		p = new Tower(preloadedObjs->at(ADVANCED_TOWER - 1), preloadedObjs->at(ADVANCED_TOWER - 1)->getOrientation(), preloadedObjs->at(ADVANCED_TOWER-1)->getTransformation());
		p->translate(tile->getPos());
		activeScene->removePieceFromAggregate(ADVANCED_TOWER, tile->getObjectID());
		tile->upgradePiece(p);
		activeScene->addPieceToAggregate(ELITE_TOWER, p);
		break;

	case ELITE:
		break;

	default:
		break;
	}

}

void Manager::removePieceFromTile(int index)
{
	Tile* tile = getScene()->getTileGrid()->getTile(index);
	
	int id = tile->getObjectID();
	int rank = tile->getRank();

	tile->removeObj();

	activeScene->removePieceFromAggregate(rank + 1, id);
}