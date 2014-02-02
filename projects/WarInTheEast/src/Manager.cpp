#include "Manager.h"

Manager::Manager()
{
	camera = new Camera();
	camera->setPerspective(tFOVY, WINDOW_WIDTH / WINDOW_HEIGHT, tNEAR, tFAR); 
	//camera->OffsetOrientation(glm::vec3(0.0, 1.0, 0.0), -45);
	//camera->OffsetOrientation(glm::vec3(1.0, 0.0, 0.0), 45);
	camera->setCenter(glm::vec3(-NUMTILESX / 2, -NUMTILESY / 2, -40.0f));
	camera->updateCamera();
	mapList = new std::vector<Scene*>;
	initMapList();
}

void Manager::tilesRayPick(float x, float y)
{
	glm::vec3 center = camera->computeCameraCenter();
	glm::vec3 view = glm::vec3(center.x, center.y, 0.0) - glm::vec3(center.x, center.y, center.z);
	view = glm::normalize(view);

	glm::fquat camQuat = camera->getQuatOrientation();
	glm::fquat camQuatConj = glm::conjugate(camQuat);

	glm::vec3 camUp = glm::vec3(camQuat * glm::vec4(0.0, 1.0f, 0.0, 1.0) * camQuatConj);
	glm::vec3 camDir = glm::vec3(camQuat * glm::vec3(0.0, 0.0f, -1.0) * camQuatConj);

	glm::vec3 h = glm::cross(view, camUp);
	h = glm::normalize(h);

	glm::vec3 v = glm::cross(h, view);
	v = glm::normalize(v);

	float radFov = (tFOVY * PI) / 180.0f;

	//Length on the viewport system
	float vLength = std::tan(radFov / 2) * (center.z);
	float hLength = vLength * (WINDOW_WIDTH / WINDOW_HEIGHT);

	v = v * vLength;
	h = h * hLength;

	x -= WINDOW_WIDTH / 2;
	y = - (y- WINDOW_HEIGHT / 2);

	x /= (WINDOW_WIDTH / 2);
	y /= (WINDOW_HEIGHT / 2);

	glm::vec3 pos = center +  view*tNEAR + h*x + v*y;

	std::cout << "MOUSE at -> " << pos.x << " "<< pos.y << " "<< pos.z << std::endl;
	glm::vec3 dir = pos - center;
	float index = std::floor(pos.x) + std::floor(pos.y)*NUMTILESX;
	if (pos.x >= 0.0f && pos.y >= 0.0f && pos.x <= NUMTILESX && pos.y <= NUMTILESY)
		activeScene->getTileGrid()->getTile(index)->setSelected();
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
	camera->setCenter(glm::vec3(camera->getCenter().x + ((x - camera->getLast_mx()) / -camera->getCenter().z), camera->getCenter().y + ((y - camera->getLast_my()) / -camera->getCenter().z), camera->getCenter().z));
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
	+++++++++++++++++++++++++++++++++++++++++++++++ CYLINDER +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	*/
	/** /
	PieceReader::getInstance().readObject("..\\objects\\Cylinder.obj");
	
	*vs = PieceReader::getInstance().getVertices();
	*is = PieceReader::getInstance().getIndices();
	p = new Piece(*vs, *is, shProg); 
	ps->push_back(p);

	PieceReader::getInstance().clearAll();
	vs->clear();
	is->clear();
	
	/**/
	/*
	+++++++++++++++++++++++++++++++++++++++++++++++  GRID  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	*/
	
	Vertex vert = *(new Vertex());
	float size = 1;
	std::vector<Tile*> *totaltiles = new std::vector<Tile*>;
	std::vector<Vertex> *tilevertexes = new std::vector<Vertex>;
	std::vector<unsigned int> *tileindexes = new std::vector<unsigned int>;

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
			vert.XYZW = glm::vec4(k, i+size, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f);  // 1
			vs->push_back(vert);
			tilevertexes->push_back(vert);



			for (int j = 0; j < 4; j++)
			{
				is->push_back(j);
			}

			p = new Tile(*vs, *is, shProg, glm::vec3(k, i, 0), scene->getId());

			totaltiles->push_back((Tile*)p);

			vs->clear();
			is->clear();
		}
	}

	for (int l = 0; l < tilevertexes->size(); l++)
	{
		tileindexes->push_back(l);
	}

	Piece* tgrid = new TileGrid(*tilevertexes, *tileindexes, shProg, *totaltiles, 0);
	ps->push_back(tgrid);

	totaltiles->clear();
	tileindexes->clear();
	tilevertexes->clear();

	/*
	+++++++++++++++++++++++++++++++++++++++++++++++           +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	*/

	return(scene);
}

Scene* Manager::getScene()
{
	return activeScene;
}

