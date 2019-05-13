#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <ioUtils.h>
#include <TileData.h>
#include <Tile.h>
class Map;
#include <Map.h>

using namespace std;

class Building {

public:
	Building(int left, int top, Map* map);
	~Building();

	int left;
	int top;
	int widthTiles;
	int heightTiles;
	Tile* mapGridTileIDs;

private:
	sf::Image* tileset;
	vector<Tile*> tiles;

	Tile* getTile(int id);
};