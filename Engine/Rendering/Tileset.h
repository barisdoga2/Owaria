#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <Tile.h>
#include <ioUtils.h>

using namespace std;

class Tileset {

public:
	Tileset(string tilesetName, string tilesetConfig, string tilesetPng);
	Tileset(const char* tilesetName, const char* pngPath, sf::Vector2f size, sf::Vector2f tilePixSize, int tilingPadding, vector<int> solidTileIDs, vector<TileData*> tileDatas) {
		this->name = tilesetName;
		this->tilesetTileSize = size;
		this->tilePixelSize = tilePixSize;
		this->tilingPadding = tilingPadding;

		this->tilesetImage = new sf::Image();
		this->tilesetImage->loadFromFile(pngPath);

		// Create Tiles
		int idCtr = 0;
		for (int y = 0; y < tilesetTileSize.y; y++) {
			for (int x = 0; x < tilesetTileSize.x; x++) {
				// Check if is id solid
				bool isSolid = false;
				for (int i : solidTileIDs)
					if (i == idCtr) {
						isSolid = true;
						break;
					}

				// Check if is id have special collision data
				TileData* tileData = nullptr;
				for (TileData* tile : tileDatas)
					if (tile->id == idCtr) {
						tileData = tile;
						break;
					}

				// Create tile
				tiles.push_back(new Tile(idCtr, *tilesetImage, sf::Vector2f((float)x, (float)y), tilePixelSize, tilingPadding, isSolid, tileData));
				idCtr++;
			}
		}
	}

	~Tileset();

	Tile* getTile(int id);
	sf::Vector2f getTilesetTileSize();
	sf::Vector2f getTilePixelSize();

	string getName();

private:
	sf::Image* tilesetImage;
	string name;

	sf::Vector2f tilesetTileSize;
	sf::Vector2f tilePixelSize;
	int tilingPadding;

	vector<Tile*> tiles;

};