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
class GameObjectSet;
#include <GameObjectSet.h>
#include <GameObject.h>

class Map {
public:
	Map(b2World* world);
	~Map();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window);
	
	sf::Vector2f m_offset;
	Tile* mapGridTileIDs;

	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);

	Tileset* getTileset();

	sf::Vector2i getMapTileSize();


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
	
	Building* building;
	Tileset* tileset;

	sf::Vector2i mapTileSize;

	GameObjectSet* treeObjectSet;
	vector<GameObject*> gameObjects;
};