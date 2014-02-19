#pragma once

#include "Includes.h"

#include "Camera.h"
#include "Scene.h"
#include "PieceReader.h"
#include "Tile.h"
#include "TileGrid.h"
#include "Interface.h"
#include "Texture2D.h"
#include "PieceInstance.h"
#include "PieceAggregate.h"
#include "Spawner.h"

enum towerTypes { NORMAL, HEAVY, SLOW };

class Manager
{
private:

	Camera *camera;
	std::vector<Scene*> *mapList;
	std::vector<Piece*> *preloadedObjs;
	Scene* activeScene;
	Interface* interface;
	PieceAggregate* x;
	Tile* starter, *obj;

public:

	Manager();

	void draw();
	void initMapList(ShaderProgram* sh);
	void initInterface(ShaderProgram* sh);
	void preLoadPieces(ShaderProgram* sh);
	void createEnemy(ShaderProgram* sh);
	void updateCameraRotation(float x, float y);
	void updateCameraPosition(float x, float y);
	void updateCameraZoom(int amount);
	void updateLastMXY(float x, float y);
	void tilesRayPick(float x, float y);

	void addPieceToTile(int index, int type);
	void upgradePieceInTile(int index);

	ShaderProgram *createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);

	Scene* getScene();

	Scene *initMap1(ShaderProgram* sh);



	//TODO
	void saveTileGrid();
	void loadTileGrid();
};