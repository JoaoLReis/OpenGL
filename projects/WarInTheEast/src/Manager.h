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
#include "Tower.h"
#include "Enemy.h"

enum towerTypes { NORMAL, HEAVY, SLOW };
enum objects {TILEGRID, NORMAL_TOWER, ADVANCED_TOWER, ELITE_TOWER, WISP};

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

	//INITIALIZATION
	void draw();
	void initMapList(ShaderProgram* sh);
	void initInterface(ShaderProgram* sh);
	void loadPiece(ShaderProgram* sh, std::string objpath, std::string texpath);
	void preLoadPieces(ShaderProgram* sh);


	//CAMERA
	void updateCameraRotation(float x, float y);
	void updateCameraPosition(float x, float y);
	void updateCameraZoom(int amount);
	void updateLastMXY(float x, float y);
	void tilesRayPick(float x, float y);

	//GAME CHANGERS
	void addPieceToTile(int index, int type);
	void upgradePieceInTile(int index);
	void createEnemy(ShaderProgram* sh);
	void removePieceFromTile(int index);

	ShaderProgram *createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);

	Scene* getScene();

	Scene *initMap1(ShaderProgram* sh);



	//TODO
	void saveTileGrid();
	void loadTileGrid();
};