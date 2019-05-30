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