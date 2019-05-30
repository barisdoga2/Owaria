#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Tileset.h>
#include <Tile.h>

using namespace std;

class BuildingAsset {

public:

	BuildingAsset(string buildingName, Tileset* tileset, string cfg) {
		this->buildingName = buildingName;
		this->tileset = tileset;

		// Load Tilemap
		int tmpTileID;
		std::ifstream infile(cfg);
		std::istringstream stream("");
		ioUtils::getNextLine(stream, infile);
		stream >> tilemapSize.x >> tilemapSize.y;
		mapGridTileIDs = (Tile*)malloc(tilemapSize.x * tilemapSize.y * sizeof(Tile));
		for (int y = 0; y < tilemapSize.y; y++) {
			ioUtils::getNextLine(stream, infile);
			for (int x = 0; x < tilemapSize.x; x++) {
				stream >> tmpTileID;
				*(mapGridTileIDs + x + tilemapSize.x * y) = *tileset->getTile(tmpTileID);
			}
		}
		infile.close();
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