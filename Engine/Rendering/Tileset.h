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
	Tileset(string tilesetConfig, string tilesetPng);
	~Tileset();

	Tile* getTile(int id);
	sf::Vector2f getTilesetTileSize();
	sf::Vector2f getTilePixelSize();

private:
	sf::Image* tilesetImage;

	sf::Vector2f tilesetTileSize;
	sf::Vector2f tilePixelSize;
	int tilingPadding;

	vector<Tile*> tiles;

};