#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Tileset.h>
#include <Tile.h>

using namespace std;

class BuildingData {

public:

	BuildingData(string buildingName) {
		this->buildingName = buildingName;

		// Load Tileset
		tileset = new Tileset("../../Resources/Buildings/" + buildingName + "/Tileset.cfg", "../../Resources/Buildings/" + buildingName + "/Tileset.png");

		// Load Tilemap
		int tmpTileID;
		std::ifstream infile("../../Resources/Buildings/" + buildingName + "/Tilemap.cfg");
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

	~BuildingData() {
		free(mapGridTileIDs);
		delete tileset;
	}

	sf::Vector2i tilemapSize;
	Tile* mapGridTileIDs;
	Tileset* tileset;
	string buildingName;

};