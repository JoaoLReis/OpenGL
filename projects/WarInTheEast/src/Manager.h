#pragma once

#include "Includes.h"

#include "Camera.h"
#include "Scene.h"
#include "PieceReader.h"
#include "Tile.h"

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
	void updateCameraRotation(float x, float y);
	void updateCameraPosition(float x, float y);
	void updateCameraZoom(int amount);

	ShaderProgram *createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);

	Scene *initMap1();
};