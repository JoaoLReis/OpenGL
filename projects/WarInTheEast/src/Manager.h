#pragma once

#include "Includes.h"

#include "Camera.h"
#include "Scene.h"
#include "PieceReader.h"
#include "Tile.h"
#include "TileGrid.h"

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
	void updateLastMXY(float x, float y);
	void tilesRayPick(float x, float y);

	ShaderProgram *createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);

	Scene* getScene();

	Scene *initMap1();
};