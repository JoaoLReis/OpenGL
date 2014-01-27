#pragma once

#include "Includes.h"

#include "Piece.h"
#include "Camera.h"
#include "Scene.h"
#include "PieceReader.h"

class Manager
{
private:

	Camera *camera;
	std::vector<Scene*> *mapList;
	Scene* activeScene;

public:

	Manager();

	void draw();
	ShaderProgram *createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
	void initMapList();
	Scene *initMap1();
};