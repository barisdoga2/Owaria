#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Box2D/Box2D.h>
#include <Tile.h>
#include <TileAsset.h>
#include <Settings.h>
#include <b2Utils.h>
class MarchingSquares;
#include <MarchingSquares.h>
#include <MarchingSolution.h>
#include <ContactData.h>
class Building;
#include <Building.h>
#include <Tileset.h>
class ObjectSet;
#include <ObjectSet.h>
#include <GameObject.h>
#include <BuildingAsset.h>
class Camera;
#include <Camera.h>
#include <tinyxml2.h>
#include <AssetStore.h>
#include <XMLUtils.h>
class TilemapEditor;
#include <TilemapEditor.h>

using namespace tinyxml2;


class Map {
public:
	Map(b2World* world, sf::RenderWindow* window, const char* xml);
	~Map();

	void Update(int updateElapsed, Camera* camera);
	void Render(sf::RenderWindow* window, Camera* camera);
	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);
	void HandleWindowEvent(sf::Event event);

	Tile* gridTiles;
	Tileset* getTileset();
	sf::Vector2i getMapSize();

private:
	MarchingSquares* marchingSquares;
	Tileset* tileset;
	vector<Building*> gameBuildings;
	vector<GameObject*> gameObjects;
	sf::Vector2i mapSize;

	b2Body* body;
	
	sf::RectangleShape* tileRenderer;
	
	TilemapEditor* tilemapEditor;
};