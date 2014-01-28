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
	void initMapList();
	void updateCameraRotation(int x, int y);

	ShaderProgram *createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);

	Scene *initMap1();
};