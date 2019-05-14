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
#include <Tileset.h>

using namespace std;

class Building {

public:
	Building(sf::Vector2i buildingTilePos, Map* map);
	~Building();

	sf::Vector2i buildingTilePos;
	sf::Vector2i buildingTileSize;

private:
	Tileset* tileset;
	Tile* mapGridTileIDs;

};