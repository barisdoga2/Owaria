#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Tileset.h>
#include <Tile.h>
#include <AssetStore.h>

using namespace std;

class BuildingAsset {

public:
	BuildingAsset(XMLElement* buildingAssetElement);
	~BuildingAsset();

	string getName();

	sf::Vector2i tilemapSize;
	Tile* mapGridTileIDs;
	Tileset* tileset;
	string buildingName;
};