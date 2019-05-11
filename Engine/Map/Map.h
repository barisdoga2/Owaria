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

class Map {
public:
	Map(b2World* world);
	~Map();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window);

	int getMapWidth();
	int getMapHeight();

	sf::Vector2f m_offset;
	int* mapGridTileIDs;

	bool isSolidTile(int tileID);

	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);

	TileData* getTileData(int id);

private:

	MarchingSquares* marchingSquares;
	sf::Image* tileset;
	std::vector<Tile*> tiles;
	std::vector<int> solidTileIDs;
	std::vector<TileData*> tileDatas;
	sf::RectangleShape* tileRenderer;

	int mapWidth;
	int mapHeight;

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
};