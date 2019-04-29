#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Box2D/Box2D.h>
#include <Tile.h>
#include <Settings.h>

class Map {
public:
	Map(b2World* world);
	~Map();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window);
private:
	sf::Image* tileset;
	std::vector<Tile*> tiles;
	sf::RectangleShape* tileRenderer;

	int mapWidth;
	int mapHeight;
	int* gridTileIDs;

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