#pragma once

#include "Includes.h"

#include "Camera.h"
#include "Scene.h"
#include "PieceReader.h"
#include "Tile.h"
#include "TileGrid.h"

enum towerTypes {NORMAL, HEAVY, SLOW};

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
	float tilesRayPick(float x, float y);

	void addPieceToTile(int index, int type);

	ShaderProgram *createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);

	Scene* getScene();

	Scene *initMap1();

	//TODO
	void saveTileGrid();
	void loadTileGrid();
};