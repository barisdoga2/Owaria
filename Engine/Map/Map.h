#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Box2D/Box2D.h>
#include <Tile.h>
#include <TileData.h>
#include <Settings.h>
#include <b2Utils.h>
class MarchingSquares;
#include <MarchingSquares.h>
#include <MarchingSolution.h>
#include <ContactData.h>
#include <ioUtils.h>
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

using namespace tinyxml2;


class Map {
public:
	Map(b2World* world, const char* xml);
	~Map();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, Camera camera);
	
	Tile* mapGridTileIDs;

	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);

	Tileset* getTileset();

	sf::Vector2i getMapTileSize();

	void AddGameObject(GameObject* gameObject);

private:

	MarchingSquares* marchingSquares;
	sf::RectangleShape* tileRenderer;

	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape s;
	b2FixtureDef fixtureDef;

	// Dynamic Physics Object for Testing
	b2BodyDef bodyDef2;
	b2Body* body2;
	b2PolygonShape p;
	b2FixtureDef fixtureDef2;
	b2Fixture* fix;
	
	Tileset* tileset;

	sf::Vector2i mapTileSize;

	vector<Building*> gameBuildings;
	vector<GameObject*> gameObjects;
};