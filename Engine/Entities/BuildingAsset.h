#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Tileset.h>
#include <Tile.h>

using namespace std;

class BuildingAsset {

public:

	BuildingAsset(string buildingName, Tileset* tileset, sf::Vector2i tilemapSize, string tileMap) {
		this->buildingName = buildingName;
		this->tileset = tileset;
		this->tilemapSize = tilemapSize;

		// Load Tilemap
		int tmpTileID;
		std::istringstream stream(tileMap);
		mapGridTileIDs = (Tile*)malloc(tilemapSize.x * tilemapSize.y * sizeof(Tile));
		for (int x = 0; x < tilemapSize.y * tilemapSize.x; x++) {
			stream >> tmpTileID;
			*(mapGridTileIDs + x) = *tileset->getTile(tmpTileID);
		}
	}

	~BuildingAsset() {
		free(mapGridTileIDs);
	}

	sf::Vector2i tilemapSize;
	Tile* mapGridTileIDs;
	Tileset* tileset;
	string buildingName;

	string getName() {
		return buildingName;
	}

};