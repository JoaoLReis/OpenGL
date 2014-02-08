#pragma once

#include "Includes.h"

#include "Camera.h"
#include "Scene.h"
#include "PieceReader.h"
#include "Tile.h"
#include "TileGrid.h"
#include "Interface.h"
#include "Texture2D.h"

enum towerTypes { NORMAL, HEAVY, SLOW };

class Manager
{
private:

	Camera *camera;
	std::vector<Scene*> *mapList;
	Scene* activeScene;

	Interface* interface;

public:

	Manager();

	void draw();
	void initMapList();
	void initInterface();
	void updateCameraRotation(float x, float y);
	void updateCameraPosition(float x, float y);
	void updateCameraZoom(int amount);
	void updateLastMXY(float x, float y);
	void tilesRayPick(float x, float y);

	void addPieceToTile(int index, int type);
	void upgradePieceInTile(int index);

	ShaderProgram *createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);

	Scene* getScene();

	Scene *initMap1();



	//TODO
	void saveTileGrid();
	void loadTileGrid();
};