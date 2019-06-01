#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <TileData.h>
#include <Tile.h>
class Map;
#include <Map.h>
#include <Tileset.h>
#include <BuildingAsset.h>

using namespace std;

class Building {

public:
	Building(BuildingAsset* BuildingAsset, sf::Vector2i buildingTilePos);
	~Building();

	void mergeWithMap(Map* map);
	sf::Vector2i getTilePosOnMap();
private:
	BuildingAsset * data;
	sf::Vector2i buildingTilePos;

};