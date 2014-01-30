#include "Manager.h"

Manager::Manager()
{
	camera = new Camera();
	camera->setPerspective(tFOVY, WINDOW_WIDTH / WINDOW_HEIGHT, tNEAR, tFAR); 
	//camera->OffsetOrientation(glm::vec3(0.0, 1.0, 0.0), -45);
	//camera->OffsetOrientation(glm::vec3(1.0, 0.0, 0.0), 45);
	camera->setEye(glm::vec3(0, 0, -40.0f));
	camera->setCenter(glm::vec3(-NUMTILESX / 2, -NUMTILESY / 2, camera->getCenter().z));
	camera->updateCamera();
	mapList = new std::vector<Scene*>;
	initMapList();
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
	camera->setCenter(glm::vec3(camera->getCenter().x + (-(x - camera->getLast_mx()) / -camera->getEye().z), camera->getCenter().y + (-(y - camera->getLast_my()) / -camera->getEye().z), camera->getCenter().z));
	camera->setLast_mx(x);
	camera->setLast_my(y);
	camera->updateCamera();
}

void Manager::updateCameraZoom(int amount)
{
	if(amount > 0)
		camera->setEye(glm::vec3(camera->getEye().x, camera->getEye().y, camera->getEye().z + 1));
	else camera->setEye(glm::vec3(camera->getEye().x, camera->getEye().y, camera->getEye().z - 1));
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

	for (int i = 0; i < NUMTILESY; i++){
		for (int k = 0; k < NUMTILESX; k++){
			vert.XYZW = glm::vec4(k, i, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 2
			vs->push_back(vert);
			vert.XYZW = glm::vec4(k, i-size, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 1.0f);  // 1
			vs->push_back(vert);
			vert.XYZW = glm::vec4(k+size, i-size, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(0.0f, 1.0f); // 0 - FRONT
			vs->push_back(vert);
			vert.XYZW = glm::vec4(k+size, i, 0.0f, 1.0f), vert.RGBA = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f), vert.NORMAL = glm::vec4(0.0f, 1.0, 0.0f, 1.0f), vert.UV = glm::vec2(1.0f, 0.0f);  // 3
			vs->push_back(vert);

			for (int j = 0; j < 4; j++)
			{
				is->push_back(j);
			}

			p = new Tile(*vs, *is, shProg, glm::vec3(k, i, 0));

			ps->push_back(p);

			vs->clear();
			is->clear();
		}
	}


	return(new Scene(ps));
}

